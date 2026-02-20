#ifndef LOCOSYNC_RESPONSE_HPP
#define LOCOSYNC_RESPONSE_HPP

#include <string>
#include <map>
#include <nlohmann/json.hpp>

namespace locosync 
{
    struct Response 
    {
        int status_code{0};
        std::string body;
        std::map<std::string, std::string> headers;
        double elapsed_time{0.0};
        std::string error_message;

        // Helper para verificar sucesso 2xx
        bool ok() const {return status_code >= 200 && status_code < 300;}

        // Parsing seguro de JSON
        nlohmann::json json() const 
        {
            try 
            {
                return nlohmann::json::parse(body);
            } 
            catch (const nlohmann::json::parse_error& e) 
            {
                return nlohmann::json::object(); // Retorna JSON vazio em caso de erro
            }
        }
    };
}
#endif // LOCOSYNC_RESPONSE_HPP