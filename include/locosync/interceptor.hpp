#ifndef LOCOSYNC_INTERCEPTOR_HPP
#define LOCOSYNC_INTERCEPTOR_HPP

#include "response.hpp"
#include <string>
#include <map>

namespace locosync 
{
    // Estrutura para Request para ser manipulada pelos interceptors
    struct Request 
    {
        std::string url;
        std::string method;
        std::map<std::string, std::string> headers;
        std::string body;
        long timeout_ms{5000}; // Timeout padrão de 5 segundos
    };

    class Interceptor 
    {
    public:
        virtual ~Interceptor() = default;

        // Executado antes da requisição sair (ex: injetar tokens de Auth)
        virtual void on_request(Request& req) = 0;

        // Executado após a resposta chegar (ex: logging ou refresh de token)
        virtual void on_response(Response& res) = 0;
    };
}
#endif // LOCOSYNC_INTERCEPTOR_HPP