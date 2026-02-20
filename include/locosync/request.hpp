#ifndef LOCOSYNC_REQUEST_HPP
#define LOCOSYNC_REQUEST_HPP

#include <string>
#include <map>

namespace locosync {

// Evitamos conflito com a keyword 'delete' usando DELETE_
enum class Method {
    GET,
    POST,
    PUT,
    DELETE_,
    PATCH
};

struct Request {
    std::string url;
    Method method{Method::GET};
    std::map<std::string, std::string> headers;
    std::string body;

    // Timeouts em milissegundos
    long timeout_ms{10000};
    long connect_timeout_ms{2000};

    bool follow_redirects{true};
    int max_redirects{5};

    std::string method_string() const {
        switch (method) {
            case Method::POST:    return "POST";
            case Method::PUT:     return "PUT";
            case Method::DELETE_: return "DELETE";
            case Method::PATCH:   return "PATCH";
            default:              return "GET";
        }
    }
};

} // namespace locosync

#endif // LOCOSYNC_REQUEST_HPP