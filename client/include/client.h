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
/**
    \brief Класс реализующий клиентское приложение.

    Данный класс предоставляет функционал для клиентского приложения.
*/
class Client
{
public:
  /**
    \brief Конструктор класса
    В конструкторе класса клиентского приложения для Windows вызывается функция WSAStartup для инициализации библиотеки Winsock
    \param [in] _name имя
    \param [in] _port номер порта
    \param [in] _period период
  */
  Client(const std::string &_name, int _port, int _period);
  /**
      \brief Деструктор класса
      В деструкторе класса клиентского приложения для Windows вызывается функция WSACleanup для завершения работы с библиотекой Winsock
  */
  ~Client();
  /** 
   * \brief Запуск клиента
  * \n - Метод start запускает бесконечный цикл, в котором периодически вызывается метод sendMessage.
  * \n - Период ожидания между отправками сообщений задается в секундах и передается через аргументы командной строки.
  */
  void start();

private:
  std::string name;
  int port;
  int period;
#ifdef _WIN32
  WSADATA wsaData;
#endif
  /** 
   * \brief Отправка сообщения
   * \n - Метод sendMessage создает новый сокет с помощью функции socket.
   * \n - Заполняется структура sockaddr_in с информацией о сервере (в данном случае используется адрес 127.0.0.1, что соответствует localhost).
   * \n - Функция connect устанавливает соединение с сервером.
   * \n - Формируется строка сообщения, содержащая текущее время и имя клиента.
   * \n - Сообщение отправляется на сервер с помощью функции send.
   * \n - Сокет закрывается с помощью функции closeSocket.
  */
  void sendMessage();
  void closeSocket(int socket);
};