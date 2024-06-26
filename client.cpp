#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <ctime>
#include <sstream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

class Client
{
    std::string name;
    int port;
    int period;
    WSADATA wsaData;

public:
    Client(const std::string &name, int port, int period)
        : name(name), port(port), period(period)
    {
        WSAStartup(MAKEWORD(2, 2), &wsaData);
    }

    ~Client()
    {
        WSACleanup();
    }

    void start()
    {
        while (true)
        {
            sendMessage();
            std::this_thread::sleep_for(std::chrono::seconds(period));
        }
    }

private:
    void sendMessage()
    {
        SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1)
        {
            perror("Socket creation error");
            return;
        }
        sockaddr_in serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
        {
            perror("Invalid address/ Address not supported");
            closesocket(sock);
            return;
        }

        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
        {
            perror("Invalid address/ Address not supported");
            closesocket(sock);
            return;
        }
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        std::stringstream ss;
        ss << "[" << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X") << "." << ms.count() << ']' << name;

        send(sock, ss.str().c_str(), ss.str().length(), 0);
        closesocket(sock);
    }
};

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << "<name> <port> <period>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string name = argv[1];
    int port = std::stoi(argv[2]);
    int period = std::stoi(argv[3]);

    Client client(name, port, period);
    client.start();

    return 0;
}