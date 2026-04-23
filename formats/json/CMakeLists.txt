add_library(${PROJECT_NAME}_json "source/serializer.cc" "source/deserializer.cc")
target_link_libraries(${PROJECT_NAME}_json PUBLIC ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}_json PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
