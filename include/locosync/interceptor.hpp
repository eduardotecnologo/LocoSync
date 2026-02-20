#ifndef LOCOSYNC_INTERCEPTOR_HPP
#define LOCOSYNC_INTERCEPTOR_HPP

#include "request.hpp"
#include "response.hpp"
#include <memory>

namespace locosync {

class Interceptor {
public:
    virtual ~Interceptor() = default;
    virtual void on_request(Request& req) {}
    virtual void on_response(Response& res) {}
};

} // namespace locosync

#endif // LOCOSYNC_INTERCEPTOR_HPP