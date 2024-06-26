#include <string>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

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
    \brief Класс реализующий сервер.

    Данный класс предоставляет функционал для сервера.
*/
class Server
{
public:
    /**
          \brief Конструктор класса
          В конструкторе класса сервера для Windows вызывается функция WSAStartup для инициализации библиотеки Winsock
          \param [in] _port номер порта
    */
    Server(int _port);
    /**
    \brief Деструктор класса
    В деструкторе класса сервера для Windows вызывается функция WSACleanup для завершения работы с библиотекой Winsock
    */
    ~Server();
    /**
     * @brief Старт сервера))
     * \n Довольно большая функция в которой происходит следуюущее:
     * \n Создание сокета:
     * \n - Создается сокет с помощью функции socket, который будет использоваться для прослушивания входящих соединений.
     * \n  Привязка сокета к порту:
     * \n Заполняется структура sockaddr_in с информацией о порте и адресе 
     * \n (в данном случае используется INADDR_ANY, что позволяет принимать соединения на всех локальных интерфейсах).
     * \n - Функция bind привязывает сокет к указанному порту.
     * \n Прослушивание входящих соединений:
     * \n - Функция listen переводит сокет в режим прослушивания, позволяя ему принимать входящие соединения.
     * \n Принятие входящих соединений:
     * \n В бесконечном цикле сервер вызывает функцию accept, которая блокирует выполнение до тех пор, пока не поступит новое входящее соединение.
     * \n - При поступлении нового соединения создается новый поток с помощью std::thread, который вызывает метод handleClient для обработки этого соединения.
    */
    void start();

private:
    /**
     * @brief номер порта
     * 
     */
    int port;
#ifdef _WIN32
    WSADATA wsaData;
#endif
    /**
     * @brief 
     * \n Метод handleClient читает данные из сокета клиента с помощью функции recv.
     * \n Если данные успешно прочитаны, они записываются в файл log.txt. Для обеспечения потокобезопасности используется мьютекс logMutex.
     * \n - Сокет клиента закрывается с помощью функции closeSocket.
     * 
    */
    void handleClient(int client_socket);
    void closeSocket(int socket);
};