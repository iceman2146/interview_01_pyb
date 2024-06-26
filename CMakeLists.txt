cmake_minimum_required(VERSION 3.29)
project(
  tcpMessenger
  VERSION 0.1.0
  LANGUAGES C CXX)
add_executable(server server.cpp)
add_executable(client client.cpp)
target_include_directories(server PRIVATE include)
target_include_directories(client PRIVATE include)
target_link_libraries(server ws2_32 wsock32)
target_link_libraries(client ws2_32 wsock32)

