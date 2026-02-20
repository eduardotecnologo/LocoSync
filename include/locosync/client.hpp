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
    // Cria uma instância compartilhada do Client
    static std::shared_ptr<Client> create();

    // Método GET síncrono (futuro: async)
    std::future<Response> get(const std::string& url);

    // Adiciona um interceptor
    void add_interceptor(std::unique_ptr<Interceptor> interceptor);

private:
    Client(); // construtor privado para controle de instância

    // Lista de interceptors
    std::vector<std::unique_ptr<Interceptor>> interceptors;

    // Funções auxiliares internas para executar a requisição
    Response perform_get(const std::string& url);
};

} // namespace locosync

#endif // LOCOSYNC_CLIENT_HPP