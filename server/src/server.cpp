#include "server.h"

std::mutex logMutex;

Server::Server(int _port) : port(_port)
{
#ifdef _WIN32
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
}

Server::~Server()
{
#ifdef _WIN32
    WSACleanup();
#endif
}

void Server::start()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
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
        closeSocket(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) == -1)
    {
        perror("listen failed");
        closeSocket(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << port << std::endl;

    while (true)
    {
        int new_socket = accept(server_fd, nullptr, nullptr);
        if (new_socket == -1)
        {
            perror("accept failed");
            continue;
        }
        std::thread(&Server::handleClient, this, new_socket).detach();
    }
}

void Server::handleClient(int client_socket)
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
    closeSocket(client_socket);
}

void Server::closeSocket(int socket)
{
#ifdef _WIN32
    closesocket(socket);
#else
    close(socket);
#endif
}
