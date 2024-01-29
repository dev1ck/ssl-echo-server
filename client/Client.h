#ifndef CLIENT_H
#define CLIENT_H

#include "SSLManager.h"

#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>
#include <stdexcept>
#include <thread>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class Client {
private:
    int _socket;
    struct sockaddr_in _server_address = {};
    SSLManager *_ssl_manager;
    SSL* _ssl;
    void receive_messages();

public:
    Client(const std::string& ip, int port, SSLManager* ssl_manager);
    ~Client();
    void connect_server();
    // void disconnect();
    void send_message(const std::string& message);
    void run();
};

#endif