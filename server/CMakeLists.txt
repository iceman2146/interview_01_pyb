add_executable(server main.cpp src/server.cpp)
target_include_directories(server PRIVATE include)
target_link_libraries(server ws2_32 wsock32)
