#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <thread>
#include <cerrno>
#include <map>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> 
#include <arpa/inet.h>

#include "IServer.h"
#include "ClientHandler.h"
#include "SSLManager.h"

class Server: public IServer
{
private:
    int _server_socket;
    struct sockaddr_in _server_address = {};
    std::thread _accept_thread;
    std::map<int, std::unique_ptr<ClientHandler>> _clients;
    SSLManager* _ssl_manager;
    void accept_clients();
    
public:
    Server(int port, SSLManager* ssl_manager);
    ~Server();
    void start(int queue_size);
    void broadcast_message(const std::string& message) override;
    void disconnect(int client_socket) override;
};

#endif