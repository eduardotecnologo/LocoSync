# 🚀 LocoSync

**[🇧🇷 Português](README.md)** | **[🇺🇸 English](README.en.md)**

> ⚠️ **Project Under Construction** - This project is under active development. APIs may change without notice.

**Crazy fast, sanely synchronous.**  
The ultimate HTTP framework for C++20, inspired by the simplicity of Axios and powered by C++ raw performance.

---

## 💡 Why LocoSync?

**LocoSync** was born to fill the gap between the complexity of C++ networking libraries (like libcurl or Boost.Asio) and the ease of use of modern high-level frameworks.

- **Native Asynchronism:** Based on `std::future` and ready for C++20 Coroutines (`co_await`).
- **First-Class JSON:** Deep integration with `nlohmann/json` for automatic parsing.
- **Interceptors:** Add headers, logs, or global authentication in a modular way.
- **Type-Safe:** Convert JSON responses directly to your C++ `structs`.
- **"Loco" Performance:** Persistent connection management (Keep-Alive) and optimized thread pool.

---

## 📦 Quick Installation

```bash
# Clone the repository
git clone https://github.com/eduardotecnologo/LocoSync
cd LocoSync

# Build with CMake
mkdir build && cd build
cmake ..
make install
```

---

## 🛠️ How to Use (The Axios Vibe)

Making a request with LocoSync is as simple as in JavaScript:

```cpp
#include <locosync/locosync.hpp>
#include <iostream>

int main() {
    auto client = locosync::Client::create();

    // Asynchronous GET Request
    client->get("https://api.github.com/users/abacus-ai")
        .then([](auto res) {
            if (res.status == 200) {
                std::cout << "Name: " << res.data["name"] << std::endl;
                std::cout << "Bio: " << res.data["bio"] << std::endl;
            }
        })
        .wait(); // Wait for completion (or use async/await)

    return 0;
}
```

### Using Interceptors (Ex: Auth Token)

**⚠️ Best Practice:** Never hardcode tokens! Use environment variables:

```cpp
#include <cstdlib>

class AuthInterceptor : public locosync::Interceptor {
private:
    std::string token;

public:
    AuthInterceptor() {
        // Get token from environment variable
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

**Usage:**

```bash
export LOCOSYNC_AUTH_TOKEN="your_token_here"
./your_application
```

---

## 🛡️ Security First

**LocoSync** was designed for developers who need a modern interface (JavaScript/Axios style) without sacrificing low-level control and extreme C++ performance.

- **TLS 1.2+ Mandatory:** Protection against downgrade attacks.
- **Strict SSL Certificate Verification:** Prevents insecure connections.
- **Protocol Restricted to HTTP/HTTPS:** Mitigates SSRF attacks via unsafe protocols.
- **Secure Memory Management:** Use of RAII and smart pointers to prevent leaks.
- **Configurable Timeouts:** Prevents requests from hanging indefinitely.

### Key Features

- **🚀 "Loco" Performance:** Persistent connection management and asynchronous execution via `std::future`.
- **🛡️ Security by Design:** TLS 1.2+ mandatory, strict SSL certificate verification, and protection against unsafe protocols.
- **🧵 Thread-Safe:** Global initialization protected by `std::once_flag`, allowing use in multi-threaded environments without race condition risks.
- **📦 First-Class JSON:** Native integration with `nlohmann/json` for sending and receiving data.
- **🔌 Powerful Interceptors:** Manipulate requests and responses globally (ideal for Auth Tokens and Logging).
- **🔍 Full Header Access:** Complete support for reading response headers (Cookies, ETag, etc.).

---

## 📁 Project Structure

```
LocoSync/
├── CMakeLists.txt                 # Build configuration file
├── include/
│   └── locosync/
│       ├── locosync.hpp           # Main header (aggregator)
│       ├── client.hpp             # HTTP client
│       ├── response.hpp           # Response structure
│       ├── request.hpp            # Request structure
│       └── interceptor.hpp        # Interceptor interface
├── src/
│   ├── client.cpp                 # Client implementation
│   └── utils.cpp                  # Utilities
├── examples/
│   └── basic_get.cpp              # Basic GET example
├── tests/
│   └── test_client.cpp            # Client tests
└── README.md                       # This file
```

---

## 🚀 Roadmap

- Full HTTP/2 support.
- Integration with simdjson for extreme performance.
- WebSocket support.
- Wrapper for C++20 Coroutines (co_await).
- Automatic Cookie manager.

---

## 🤝 Contributing

Feel free to open Issues or send Pull Requests. Let's make the C++ ecosystem friendlier together!

---

## 📄 License

Distributed under the MIT License. See LICENSE for more information.
