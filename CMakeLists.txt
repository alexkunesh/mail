cmake_minimum_required(VERSION 3.30)
project(mail C CXX)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

add_subdirectory("mail")
add_subdirectory("formats/binary")
add_subdirectory("formats/json")
#add_subdirectory("formats/toml")
add_subdirectory("samples")