#ifndef LOCOSYNC_MAIN_HPP
#define LOCOSYNC_MAIN_HPP

// Agregador de todos os componentes do framework
#include "client.hpp"
#include "request.hpp"
#include "response.hpp"
#include "interceptor.hpp"

namespace locosync {
    // Versão do Framework para auditoria e telemetria
    inline const std::string VERSION = "0.1.0-alpha";
}

#endif