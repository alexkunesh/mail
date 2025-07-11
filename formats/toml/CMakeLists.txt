
FetchContent_Declare(
        toml11
        GIT_REPOSITORY https://github.com/ToruNiina/toml11.git
        GIT_TAG v4.4.0
)
FetchContent_MakeAvailable(toml11)

add_library(${PROJECT_NAME}_toml "source/ser.cc" "source/de.cc")
target_link_libraries(${PROJECT_NAME}_toml PUBLIC ${PROJECT_NAME} toml11)
target_include_directories(${PROJECT_NAME}_toml PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
