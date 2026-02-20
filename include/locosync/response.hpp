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

    bool ok() const {
        return status_code >= 200 && status_code < 300 && error_message.empty();
    }

    nlohmann::json json() const {
        try {
            if (body.empty()) return nlohmann::json::object();
            return nlohmann::json::parse(body);
        } catch (const nlohmann::json::parse_error& e) {
            return nlohmann::json({{"error", "invalid_json"}, {"details", e.what()}});
        }
    }

    std::string get_header(const std::string& name) const {
        auto it = headers.find(name);
        if (it != headers.end()) return it->second;
        return {};
    }
};

} // namespace locosync

#endif // LOCOSYNC_RESPONSE_HPP