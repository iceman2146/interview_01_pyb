#include <chrono>
#include <string>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #pragma comment(lib, "Ws2_32.lib")
#else
  #include <arpa/inet.h>
  #include <sys/socket.h>
  #include <unistd.h>
#endif

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
#ifdef _WIN32
  WSADATA wsaData;
#endif
  void sendMessage();
  void closeSocket(int socket);
};