#include "locosync/locosync.hpp"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    std::cout << "🚀 LocoSync Pokémon List Example" << std::endl;
    std::cout << "Framework Version: " << LOCOSYNC_VERSION << std::endl;
    std::cout << "--------------------------------" << std::endl;

    auto client = locosync::Client::create();

    const std::string url = "https://pokeapi.co/api/v2/pokemon?limit=20&offset=0";
    auto future_res = client->get(url);

    std::cout << "Buscando lista de Pokémons..." << std::endl;

    // Simulamos trabalho enquanto a requisição é processada
    for (int i = 0; i < 3; ++i) {
        std::cout << "Trabalhando... " << (i+1) << "s" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    locosync::Response res = future_res.get();

    if (res.ok()) {
        auto data = res.json();
        std::cout << "Total de Pokémons disponíveis: " << data.value("count", 0) << std::endl;
        auto results = data["results"];
        std::cout << "Primeiros 20 Pokémons:" << std::endl;
        for (const auto& pokemon : results) {
            std::string name = pokemon.value("name", "N/A");
            std::string url = pokemon.value("url", "N/A");
            std::cout << " - " << name << " (" << url << ")" << std::endl;
        }

    } else {
        std::cerr << "Erro ao buscar Pokémons: " << res.error_message << std::endl;
    }

    std::cout << "--------------------------------" << std::endl;
    std::cout << "Fim do exemplo." << std::endl;
    return 0;
}