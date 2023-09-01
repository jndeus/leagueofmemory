cmake_minimum_required(VERSION 3.26)
project(leagueofmemory)

set(CMAKE_CXX_STANDARD 14)

# Adicione o diretório das imagens ao executável
include_directories(
        ${CMAKE_SOURCE_DIR}/imagens
        ${CMAKE_SOURCE_DIR}/font
)
# Defina as fontes do seu projeto
set(SOURCES
        main.c
        # ... adicione mais arquivos fonte, se necessário
)

# Defina o executável
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(leagueofmemory allegro allegro_font allegro_image allegro_ttf)

# Copie as imagens para o diretório de compilação
file(COPY ${CMAKE_SOURCE_DIR}/imagens DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/font DESTINATION ${CMAKE_BINARY_DIR})