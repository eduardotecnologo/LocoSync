cmake_minimum_required(VERSION 3.16)
project(LocoSync VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Opção para build em modo Release por padrão
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Dependências externas
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

# Inclui diretórios de headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Biblioteca LocoSync
add_library(locosync STATIC
    src/client.cpp
    src/utils.cpp
)

target_link_libraries(locosync PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)

target_include_directories(locosync PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Exemplo executável
add_executable(example_basic examples/basic_get.cpp)
target_link_libraries(example_basic PRIVATE locosync)
# Se estiver usando vcpkg
set(CMAKE_TOOLCHAIN_FILE "C:/Users/EduDevWin/Documents/GIT/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")