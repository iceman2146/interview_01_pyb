add_executable(client main.cpp src/client.cpp)
target_include_directories(client PRIVATE include)
if(WIN32)
    target_link_libraries(client ws2_32 wsock32)
endif()