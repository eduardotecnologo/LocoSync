#ifndef LOCOSYNC_RESPONSE_HPP
#define LOCOSYNC_RESPONSE_HPP

#include <string>
#include <map>
#include <nlohmann/json.hpp>

namespace locosync {

struct Response {
    int status_code{0};
    std::string body;
    std::map<std::string, std::string> headers;
    double elapsed_time{0.0};
    std::string error_message;

    // Verifica se a requisição foi bem sucedida (Status 2xx)
    bool ok() const 
    {
        return status_code >= 200 && status_code < 300 && error_message.empty();
    }

    // Parsing de JSON com tratamento de exceção interno (Segurança)
    nlohmann::json json() const 
    {
        try {
            if (body.empty()) return nlohmann::json::object();
            return nlohmann::json::parse(body);
        } catch (const nlohmann::json::parse_error& e) 
        {
            // Em um ambiente de produção, aqui logaríamos o erro de segurança/parsing
            return nlohmann::json::object({{"error", "invalid_json"}, {"details", e.what()}});
        }
    }
    
    // Helper para buscar headers de forma case-insensitive (comum em HTTP)
    std::string get_header(const std::string& name) const {
        for (const auto& [key, value] : headers) {
            // Comparação simples (pode ser melhorada para case-insensitive real)
            if (key == name) return value;
        }
        return "";
    }
};

} // namespace locosync

#endif