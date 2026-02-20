#include "locosync/client.hpp"
#include <curl/curl.h>
#include <mutex>
#include <algorithm>
#include <cctype>

namespace locosync {

// Garantia de inicialização única e segura do motor cURL
static std::once_flag curl_init_flag;

// Callback para capturar o corpo da resposta
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    auto* body = static_cast<std::string*>(userp);
    try {
        body->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    } catch (...) { return 0; }
}

// Callback para capturar e parsear Headers de resposta com segurança
static size_t HeaderCallback(char* buffer, size_t size, size_t nitems, void* userdata) {
    size_t totalSize = size * nitems;
    std::string headerLine(buffer, totalSize);
    auto* headers = static_cast<std::map<std::string, std::string>*>(userdata);

    size_t colonPos = headerLine.find(':');
    if (colonPos != std::string::npos) {
        std::string key = headerLine.substr(0, colonPos);
        std::string value = headerLine.substr(colonPos + 1);

        // Trim básico para remover espaços e \r\n
        auto trim = [](std::string& s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
            s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
        };
        trim(key); trim(value);
        (*headers)[key] = value;
    }
    return totalSize;
}

std::shared_ptr<Client> Client::create() {
    // Usamos o construtor privado via helper para manter o encapsulamento
    struct MakeSharedEnabler : public Client {};
    return std::make_shared<MakeSharedEnabler>();
}

Client::Client() {
    std::call_once(curl_init_flag, []() {
        curl_global_init(CURL_GLOBAL_DEFAULT);
    });
}

Client::~Client() {
    // Nota: curl_global_cleanup geralmente é chamado apenas no fim do programa.
    // Em frameworks, deixamos o SO limpar ou controlamos via ref-count global.
}

std::future<Response> Client::request(const Request& req) {
    return std::async(std::launch::async, [this, req]() -> Response {
        Request mutable_req = req;

        // 1. Interceptors de Saída
        for (auto& i : interceptors) { if (i) i->on_request(mutable_req); }

        Response res_obj;
        CURL* curl = curl_easy_init();
        struct curl_slist* header_list = nullptr;

        if (!curl) {
            res_obj.error_message = "Critical: Could not initialize cURL handle.";
            return res_obj;
        }

        // --- HARDENING DE SEGURANÇA ---
        curl_easy_setopt(curl, CURLOPT_URL, mutable_req.url.c_str());
        curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2); // Mínimo TLS 1.2
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
        
        // Proteção contra ataques de negação de serviço (Timeouts)
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, mutable_req.timeout_ms);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, mutable_req.connect_timeout_ms);
        
        // Configuração do Método
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, mutable_req.method_string().c_str());

        // Headers de Saída
        for (const auto& [key, value] : mutable_req.headers) {
            std::string h = key + ": " + value;
            header_list = curl_slist_append(header_list, h.c_str());
        }

        // Payload (Body)
        if (!mutable_req.body.empty()) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, mutable_req.body.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, static_cast<long>(mutable_req.body.size()));
            
            // Default para JSON se não especificado (User-friendly)
            if (mutable_req.headers.find("Content-Type") == mutable_req.headers.end()) {
                header_list = curl_slist_append(header_list, "Content-Type: application/json");
            }
        }

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

        // Callbacks de Resposta
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res_obj.body);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HeaderCallback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &res_obj.headers);

        // Execução
        CURLcode code = curl_easy_perform(curl);

        if (code != CURLE_OK) {
            res_obj.error_message = curl_easy_strerror(code);
        } else {
            long http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            res_obj.status_code = static_cast<int>(http_code);

            double t = 0.0;
            curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &t);
            res_obj.elapsed_time = t;
        }

        // Limpeza Segura
        if (header_list) curl_slist_free_all(header_list);
        curl_easy_cleanup(curl);

        // 2. Interceptors de Entrada
        for (auto& i : interceptors) { if (i) i->on_response(res_obj); }

        return res_obj;
    });
}

// Implementação dos Shorthands
std::future<Response> Client::get(const std::string& url) {
    Request r; r.url = url; r.method = Method::GET; return request(r);
}

std::future<Response> Client::post(const std::string& url, const nlohmann::json& body) {
    Request r; r.url = url; r.method = Method::POST; r.body = body.dump(); return request(r);
}

std::future<Response> Client::put(const std::string& url, const nlohmann::json& body) {
    Request r; r.url = url; r.method = Method::PUT; r.body = body.dump(); return request(r);
}

std::future<Response> Client::del(const std::string& url) {
    Request r; r.url = url; r.method = Method::DELETE; return request(r);
}

void Client::add_interceptor(std::unique_ptr<Interceptor> interceptor) {
    interceptors.push_back(std::move(interceptor));
}

} // namespace locosync