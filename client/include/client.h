#include <chrono>
#include <string>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

class Client
{
public:
  Client(const std::string &_name, int _port, int _period);

  ~Client();

  void start();

private:
  std::string name;
  int port;
  int period;
  WSADATA wsaData;
  void sendMessage();
};