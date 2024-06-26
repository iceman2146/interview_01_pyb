#include "server.h"
#ifdef _WIN32
  #define _CRT_SECURE_NO_WARNINGS
#endif
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