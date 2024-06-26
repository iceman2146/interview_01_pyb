#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <mutex>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

std::mutex logMutex;

class Server
{
    int port;
    WSADATA wsaData;

public:
    Server(int port) : port(port)
    {
        WSAStartup(MAKEWORD(2, 2), &wsaData);
    }

    ~Server()
    {
        WSACleanup();
    }

    void start()
    {
        SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1)
        {
            perror("bind failed");
            closesocket(server_fd);
            exit(EXIT_FAILURE);
        }

        if (listen(server_fd, 3) == -1)
        {
            perror("listen failed");
            closesocket(server_fd);
            exit(EXIT_FAILURE);
        }

        std::cout << "Server listening on port " << port << std::endl;

        while (true)
        {
            SOCKET new_socket = accept(server_fd, nullptr, nullptr);
            if (new_socket == INVALID_SOCKET)
            {
                perror("accept failed");
                continue;
            }
            std::thread(&Server::handleClient, this, new_socket).detach();
        }
    }

private:
    void handleClient(SOCKET client_socket)
    {
        char buffer[1024] = {0};
        int valread = recv(client_socket, buffer, 1024, 0);
        if (valread > 0)
        {
            std::lock_guard<std::mutex> lock(logMutex);
            std::ofstream logFile("log.txt", std::ios::app);
            logFile << buffer << std::endl;
            logFile.close();
        }
        closesocket(client_socket);
    }
};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return EXIT_FAILURE;
    }

    int port = std::stoi(argv[1]);
    Server server(port);
    server.start();

    return 0;
}