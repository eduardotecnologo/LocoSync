cmake_minimum_required(VERSION 3.16)
project(LocoSync VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Encontrar dependências
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

# Biblioteca LocoSync
add_library(locosync STATIC
    src/client.cpp
    src/utils.cpp
)

# Linkar libcurl e nlohmann_json na biblioteca
#target_link_libraries(locosync PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)
# nlohmann_json é usado nas headers públicas (ex: response.hpp), portanto deve ser PUBLIC
target_link_libraries(locosync
    PRIVATE CURL::libcurl
    PUBLIC  nlohmann_json::nlohmann_json
)

# Incluir headers publicamente para quem linkar com locosync
target_include_directories(locosync PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Executável exemplo Pokémon
add_executable(pokemon_example examples/list_pokemons.cpp)

# Linkar locosync no executável
target_link_libraries(pokemon_example PRIVATE locosync)

# Garantir que o executável encontre os headers do projeto
target_include_directories(pokemon_example PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Diretório de saída para executáveis (opcional)
set_target_properties(pokemon_example PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)