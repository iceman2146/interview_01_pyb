#include "client.h"

Client::Client(const std::string &_name, int _port, int _period)
    : name(_name), port(_port), period(_period)
{
#ifdef _WIN32
  WSAStartup(MAKEWORD(2, 2), &wsaData);//инициализации библиотеки Winsock
#endif
}

Client::~Client()
{
#ifdef _WIN32
  WSACleanup();//завершения работы с библиотекой Winsock
#endif
}

void Client::start()
{
  while (true)
  {
    sendMessage();
    std::this_thread::sleep_for(std::chrono::seconds(period));
  }
}

void Client::sendMessage()
{
  int sock = socket(AF_INET, SOCK_STREAM, 0);
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
    closeSocket(sock);
    return;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
  {
    perror("Connection Failed");
    closeSocket(sock);
    return;
  }

  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()) %
            1000;

  std::stringstream ss;
  ss << "[" << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X") << "."
     << ms.count() << "] " << name;

  send(sock, ss.str().c_str(), ss.str().length(), 0);
  closeSocket(sock);
}

void Client::closeSocket(int socket)
{
#ifdef _WIN32
  closesocket(socket);
#else
  close(socket);
#endif
}
