#ifndef CLIENT_HANDLER_h
#define CLIENT_HANDLER_h

#include <thread>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstring>
#include <cerrno>
// #include <mutex>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "IServer.h"
#include "SSLManager.h"

class ClientHandler
{
private:
    int _client_socket;
    struct sockaddr_in _client_address;
    std::thread _client_thread;
    IServer* _server;
    SSLManager* _ssl_manager;
    SSL *_ssl;
    // std::mutex _ssl_mutex;
    
public:
    ClientHandler(int socket, struct sockaddr_in address, IServer* server, SSLManager* ssl_manager);
    void handle();
    void send_message(const std::string& message);
    void stop();
};

#endif

