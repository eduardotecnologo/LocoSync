#ifndef LOCOSYNC_REQUEST_HPP
#define LOCOSYNC_REQUEST_HPP

#include <string>
#include <map>
#include <vector>

namespace locosync 
{
    enum class Method 
    {GET,POST,PUT,DELETE, PATCH,HEAD,OPTIONS};

    struct Request 
    {
        std::string url;
        Method method{Method::GET};
        std::map<std::string, std::string> headers;
        std::string body;

        // Segurança: Timeouts agressivos por padrão para evitar ataques de DoS (Slowloris)
        long timeout_ms{10000}; // Timeout padrão de 10 segundos
        long connect_timeout_ms{2000}; // 2s para estabelecer conexão

        bool follow_redirects{true}; 
        int max_redirects{5}; // Limite de redirecionamentos para evitar loops infinitos

        std::string method_string() const
        {
            switch(method) 
            {
                case Method::POST: return "POST";
                case Method::PUT: return "PUT";
                case Method::DELETE: return "DELETE";
                case Method::PATCH: return "PATCH";
                case Method::HEAD: return "HEAD";
                case Method::OPTIONS: return "OPTIONS";
                default: return "GET";
            }
        }
    };
}

#endif