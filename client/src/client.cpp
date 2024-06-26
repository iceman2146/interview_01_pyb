#include "client.h"

Client::Client(const std::string &_name, int _port, int _period)
    : name(_name), port(_port), period(_period)
{
  WSAStartup(MAKEWORD(2, 2), &wsaData);
}

Client::~Client()
{
  WSACleanup();
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
    closesocket(sock);
    return;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
  {
    perror("Connection Failed");
    closesocket(sock);
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
  closesocket(sock);
}

