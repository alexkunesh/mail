cmake_minimum_required(VERSION 3.30)
project(mail C CXX)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

# Enable testing
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)

    # Add tests directory if BUILD_TESTING is enabled.
    if (BUILD_TESTING)
        add_subdirectory("tests")
    endif ()
endif ()

add_subdirectory("mail")
add_subdirectory("formats/binary")
add_subdirectory("formats/json")
add_subdirectory("samples")