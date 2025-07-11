add_library(test_gen "test-gen.cc")
target_link_libraries(test_gen PUBLIC ${PROJECT_NAME})

add_executable(${PROJECT_NAME}_sample_test test.cc)
target_link_libraries(${PROJECT_NAME}_sample_test ${PROJECT_NAME} ${PROJECT_NAME}_json ${PROJECT_NAME}_toml ${PROJECT_NAME}_binary test_gen)

add_executable(${PROJECT_NAME}_binary_test binary-test.cc)
target_link_libraries(${PROJECT_NAME}_binary_test ${PROJECT_NAME} ${PROJECT_NAME}_binary)