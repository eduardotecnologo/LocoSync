#ifndef LOCOSYNC_CLIENT_HPP
#define LOCOSYNC_CLIENT_HPP

#include <string>
#include <future>
#include <memory>
#include "response.hpp"
#include "interceptor.hpp"

namespace locosync {

class Client {
public:
    // Método genérico (O coração do framework)
    std::future<Response> request(const Request& req);

    // Atalhos (Shorthands)
    std::future<Response> get(const std::string& url);
    std::future<Response> post(const std::string& url, const nlohmann::json& body);
    std::future<Response> put(const std::string& url, const nlohmann::json& body);
    std::future<Response> del(const std::string& url); // 'delete' é palavra reservada em C++

private:
    Client(); // construtor privado para controle de instância

    // Lista de interceptors
    std::vector<std::unique_ptr<Interceptor>> interceptors;

    // Funções auxiliares internas para executar a requisição
    Response perform_get(const std::string& url);
};

} // namespace locosync

#endif // LOCOSYNC_CLIENT_HPP