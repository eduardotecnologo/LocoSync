#include "locosync/client.hpp"
#include <curl/curl.h>
#include <iostream>

namespace locosync {
    // Callback estático para escrita de dados (Seguro contra estouro)    
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) 
    {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    std::shared_ptr<Client> Client::create() 
    {
        return std::shared_ptr<Client>(new Client());
    }

    Client::Client() 
    {
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }

    std::future<Response> Client::get(const std::string& url)
    {   // Encapsula a execução em uma thread separada (Async)
        return std::async(std::launch::async, [this, url]()
        {
            return this->perform_get(url);
        });
    }

    Response Client::perform_get(const std::string& url) 
    {
        Request req;
        req.url = url;
        req.method = "GET";

        // 1. Rodar Interceptors de Request

        for(auto& i : interceptors)
        {
            i->on_request(req);
        }
        Response res_obj;
        CURL* curl = curl_easy_init();
        
        if(curl) 
        {   
        // --- SEGURANÇA ---
        curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L); // Forçar verificação SSL
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, req.timeout_ms);
        // -----------------
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res_obj.body);   
        
        CURLcode res = curl_easy_perform(curl);

            if(res != CURLE_OK) 
            {
                res_obj.error_message = curl_easy_strerror(res);
            } 
            else 
            {
                long http_code = 0;
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
                res_obj.status_code = static_cast<int>(http_code);
            }
            curl_easy_cleanup(curl);
        }
        // 2. Rodar Interceptors de Response
        for(auto& i : interceptors)
        {
            i->on_response(res_obj);
        }
        return res_obj;
    }
    void Client::add_interceptor(std::unique_ptr<Interceptor> interceptor) 
    {
        interceptors.push_back(std::move(interceptor));
    }
}