#include <string>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
#else
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <unistd.h>
#endif

class Server
{
public:
    Server(int _port);
    ~Server();
    void start();

private:
    int port;
#ifdef _WIN32
    WSADATA wsaData;
#endif

    void handleClient(int client_socket);
    void closeSocket(int socket);
};