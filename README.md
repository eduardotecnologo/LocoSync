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
```

---

🛡️ Segurança em Primeiro Lugar
TLS 1.2+ obrigatório: Proteção contra ataques de downgrade.
Verificação rigorosa de certificados SSL: Evita conexões inseguras.
Protocolo restrito a HTTP/HTTPS: Mitiga ataques SSRF via protocolos inseguros.
Gerenciamento seguro de memória: Uso de RAII e smart pointers para evitar vazamentos.
Timeouts configuráveis: Evita que requisições travem indefinidamente.

## 📁 Estrutura do Projeto

```
LocoSync/
├── CMakeLists.txt                 # Arquivo de configuração do build
├── include/
│   └── locosync/
│       ├── locosync.hpp           # Header principal
│       ├── client.hpp
│       ├── response.hpp
│       ├── request.hpp
│       └── interceptor.hpp
├── src/
│   ├── client.cpp
│   ├── utils.cpp
│   └── ...
├── examples/
│   └── basic_get.cpp
├── tests/
│   └── test_client.cpp
└── README.md
```

---

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
