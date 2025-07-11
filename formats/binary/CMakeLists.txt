add_library(${PROJECT_NAME}_binary "source/ser.cc" "source/de.cc")
target_link_libraries(${PROJECT_NAME}_binary PUBLIC ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}_binary PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
