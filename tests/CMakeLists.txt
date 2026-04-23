# Include GoogleTest
# For Windows: Prevent overriding the parent project's compiler/linker settings
# Idk; see https://google.github.io/googletest/quickstart-cmake.html (as of 07/22/25)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()
file(GLOB_RECURSE SOURCES "source/*.cc")
add_executable(${PROJECT_NAME}_tests ${SOURCES})
target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME} ${PROJECT_NAME}_binary ${PROJECT_NAME}_json GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)