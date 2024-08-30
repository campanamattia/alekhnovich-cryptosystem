cmake_minimum_required(VERSION 3.10)
project(my_project)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Aggiungi la directory degli header
include_directories(code/include)

# Aggiungi i sorgenti (correggi il percorso)
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.c")

# Verifica se i sorgenti sono stati trovati
if(NOT SOURCES)
    message(FATAL_ERROR "No source files found in ${CMAKE_SOURCE_DIR}/src/")
endif()

# Crea l'eseguibile con il nome corretto
add_executable(MyProject ${SOURCES})

# Collega la libreria randombytes usando il nome corretto del target
target_link_libraries(MyProject randombytes)

# Aggiungi il percorso della libreria randombytes
link_directories(/usr/local/lib)

# Imposta la variabile d'ambiente LD_LIBRARY_PATH
set(ENV{LD_LIBRARY_PATH} "/usr/local/lib:$ENV{LD_LIBRARY_PATH}")

# Specifica la cartella di output per l'eseguibile
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
