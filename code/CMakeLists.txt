cmake_minimum_required(VERSION 3.10)
project(MyProject VERSION 1.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Aggiungi la directory degli header
include_directories(include)

# Aggiungi i sorgenti
file(GLOB SOURCES "src/*.c")

add_executable(MyProject ${SOURCES})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
