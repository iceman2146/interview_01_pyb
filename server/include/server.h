#include <string>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>


    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")


class Server
{
public:
    Server(int _port);
    ~Server();
    void start();

private:
    int port;
    WSADATA wsaData;


    void handleClient(int client_socket);

};