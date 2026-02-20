# 🚀 LocoSync

**[🇧🇷 Português](README.md)** | **[🇺🇸 English](README.en.md)**

> ⚠️ **Projeto em Construção** - Este projeto ainda está em desenvolvimento ativo. As APIs podem mudar sem aviso prévio.

**Crazy fast, sanely synchronous.**  
O framework HTTP definitivo para C++20, inspirado na simplicidade do Axios e movido pela performance bruta do C++.

---

## 💡 Por que LocoSync?

O **LocoSync** nasceu para preencher a lacuna entre a complexidade das bibliotecas de rede em C++ (como libcurl ou Boost.Asio) e a facilidade de uso de frameworks modernos de alto nível.

- **Assincronismo Nativo:** Baseado em `std::future` e pronto para Corrotinas C++20 (`co_await`).
- **JSON de Primeira Classe:** Integração profunda com `nlohmann/json` para parsing automático.
- **Interceptors:** Adicione headers, logs ou autenticação global de forma modular.
- **Type-Safe:** Converta respostas JSON diretamente para suas `structs` C++.
- **Performance "Loco":** Gerenciamento de conexões persistentes (Keep-Alive) e pool de threads otimizado.

---

## 📦 Instalação Rápida

```bash
# Clone o repositório
git clone https://github.com/eduardotecnologo/LocoSync
cd LocoSync

# Build com CMake
mkdir build && cd build
cmake ..
make install
```

---

## 🛠️ Como usar (The Axios Vibe)

Fazer uma requisição com o LocoSync é tão simples quanto no JavaScript:

```cpp
#include <locosync/locosync.hpp>
#include <iostream>

int main() {
    auto client = locosync::Client::create();

    // Requisição GET Assíncrona
    client->get("https://api.github.com/users/abacus-ai")
        .then([](auto res) {
            if (res.status == 200) {
                std::cout << "Nome: " << res.data["name"] << std::endl;
                std::cout << "Bio: " << res.data["bio"] << std::endl;
            }
        })
        .wait(); // Aguarda a conclusão (ou use async/await)

    return 0;
}
```

### Usando Interceptors (Ex: Auth Token)

**⚠️ Melhor Prática:** Nunca hardcode tokens! Use variáveis de ambiente:

```cpp
#include <cstdlib>

class AuthInterceptor : public locosync::Interceptor {
private:
    std::string token;

public:
    AuthInterceptor() {
        // Obtém o token da variável de ambiente
        const char* env_token = std::getenv("LOCOSYNC_AUTH_TOKEN");
        if (env_token) {
            token = std::string(env_token);
        }
    }

    void on_request(locosync::Request& req) override {
        if (!token.empty()) {
            req.headers["Authorization"] = "Bearer " + token;
        }
    }
};

client->add_interceptor(std::make_unique<AuthInterceptor>());
```

**Usar assim:**

```bash
export LOCOSYNC_AUTH_TOKEN="seu_token_aqui"
./seu_aplicativo
```

---

## 🛡️ Segurança em Primeiro Lugar

O **LocoSync** foi projetado para desenvolvedores que precisam de uma interface moderna (estilo JavaScript/Axios) sem abrir mão do controle de baixo nível e da performance extrema do C++.

- **TLS 1.2+ Obrigatório:** Proteção contra ataques de downgrade.
- **Verificação Rigorosa de Certificados SSL:** Evita conexões inseguras.
- **Protocolo Restrito a HTTP/HTTPS:** Mitiga ataques SSRF via protocolos inseguros.
- **Gerenciamento Seguro de Memória:** Uso de RAII e smart pointers para evitar vazamentos.
- **Timeouts Configuráveis:** Evita que requisições travem indefinidamente.

### Recursos Principais

- **🚀 Performance "Loco":** Gerenciamento de conexões persistentes e execução assíncrona via `std::future`.
- **🛡️ Security by Design:** TLS 1.2+ obrigatório, verificação rigorosa de certificados SSL e proteção contra protocolos inseguros.
- **🧵 Thread-Safe:** Inicialização global protegida por `std::once_flag`, permitindo uso em ambientes multi-thread sem riscos de race conditions.
- **📦 JSON de Primeira Classe:** Integração nativa com `nlohmann/json` para envio e recebimento de dados.
- **🔌 Interceptors Potentes:** Manipule requisições e respostas globalmente (ideal para Auth Tokens e Logging).
- **🔍 Full Header Access:** Suporte completo para leitura de headers de resposta (Cookies, ETag, etc.).

---

## 📁 Estrutura do Projeto

```
LocoSync/
├── CMakeLists.txt                 # Arquivo de configuração do build
├── include/
│   └── locosync/
│       ├── locosync.hpp           # Header principal (agregador)
│       ├── client.hpp             # Cliente HTTP
│       ├── response.hpp           # Estrutura de resposta
│       ├── request.hpp            # Estrutura de requisição
│       └── interceptor.hpp        # Interface de interceptores
├── src/
│   ├── client.cpp                 # Implementação do cliente
│   └── utils.cpp                  # Utilitários
├── examples/
│   └── basic_get.cpp              # Exemplo básico de GET
├── tests/
│   └── test_client.cpp            # Testes do cliente
└── README.md                       # Este arquivo
```

---

## 🛠️ Como usar LocoSync, agora na prática!

### 1. Requisição GET Simples (Assíncrona)

```cpp
#include <locosync/locosync.hpp>
#include <iostream>

int main() {
    auto client = locosync::Client::create();

    // O método .get() retorna um std::future<Response>
    auto future_res = client->get("https://pokeapi.co/api/v2/pokemon?limit=20&offset=0");

    // Faça outras coisas enquanto a requisição processa...

    auto res = future_res.get(); // Aguarda o resultado
    if (res.ok()) {
        std::cout << "User: " << res.json()["name"] << std::endl;
    }
    return 0;
}
```

### 2. Requisição POST com JSON (Estilo Axios)

```cpp
auto body = nlohmann::json{{"username", "admin"}, {"password", "12345"}};

client->post("https://api.exemplo.com/login", body)
    .then([](auto res) {
        if (res.status_code == 200) {
            // Acesso fácil aos headers de resposta
            std::string token = res.headers["Authorization"];
            std::cout << "Login realizado! Token: " << token << std::endl;
        }
    });
```

### 3. Interceptors (Segurança e Automação)

```cpp
class SecurityInterceptor : public locosync::Interceptor {
    void on_request(locosync::Request& req) override {
        req.headers["X-Custom-Security-Header"] = "LocoSync-Protected";
    }
    void on_response(locosync::Response& res) override {
        if (res.status_code == 401) {
            std::cerr << "Alerta de Segurança: Acesso não autorizado!" << std::endl;
        }
    }
};

client->add_interceptor(std::make_unique<SecurityInterceptor>());
```

## 🛡️ Hardening de Segurança

O LocoSync implementa práticas recomendadas de Segurança da Informação:

- **TLS Hardening**: Desabilita versões antigas de SSL/TLS (SSLv2, SSLv3, TLS 1.0/1.1) para prevenir ataques de interceptação.
- **Memory Safety**: Uso rigoroso de RAII e Smart Pointers para garantir que headers e handles do cURL sejam liberados, prevenindo vazamentos de memória.
- **Protocol Lockdown**: Apenas http:// e https:// são permitidos, mitigando ataques de SSRF.
- **Timeout Enforcement**: Timeouts de conexão e transferência obrigatórios para evitar ataques de Slowloris.

## 🚀 Roadmap

- Suporte completo a HTTP/2.
- Integração com simdjson para performance extrema.
- Suporte a WebSockets.
- Wrapper para Corrotinas C++20 (co_await).
- Gerenciador de Cookies automático.

---

## 🤝 Contribuição

Sinta-se à vontade para abrir Issues ou enviar Pull Requests. Vamos tornar o ecossistema C++ mais amigável juntos!

---

## 📄 Licença

Distribuído sob a licença MIT. Veja LICENSE para mais informações.
