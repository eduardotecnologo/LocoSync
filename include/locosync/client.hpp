#ifndef LOCOSYNC_CLIENT_HPP
#define LOCOSYNC_CLIENT_HPP

#include "request.hpp"
#include "response.hpp"
#include "interceptor.hpp"

#include <string>
#include <future>
#include <memory>
#include <vector>
#include <nlohmann/json.hpp>

namespace locosync {

class Client {
public:
    // Factory
    static std::shared_ptr<Client> create();

    // Destrutor (declarado para permitir definição no .cpp)
    virtual ~Client();

    // Método genérico
    std::future<Response> request(const Request& req);

    // Conveniências
    std::future<Response> get(const std::string& url);
    std::future<Response> post(const std::string& url, const nlohmann::json& body);
    std::future<Response> put(const std::string& url, const nlohmann::json& body);
    std::future<Response> del(const std::string& url);

    // Interceptors
    void add_interceptor(std::unique_ptr<Interceptor> interceptor);

protected:
    Client();

private:
    std::vector<std::unique_ptr<Interceptor>> interceptors;
};

} // namespace locosync

#endif // LOCOSYNC_CLIENT_HPP