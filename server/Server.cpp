#include "Server.h"


Server::Server(int port, SSLManager *ssl_manager): _ssl_manager(ssl_manager)
{
    if ((_server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        throw std::runtime_error("[socket]: " + std::string(strerror(errno)));
    }

    _server_address.sin_family = AF_INET;
    _server_address.sin_addr.s_addr = INADDR_ANY;
    _server_address.sin_port = htons(port);
}

void Server::start(int queue_size)
{
    if (bind(_server_socket, reinterpret_cast<sockaddr *>(&_server_address), sizeof(_server_address)) == -1)
    {
        throw std::runtime_error("[bind]: " + std::string(strerror(errno)));
    }

    if (listen(_server_socket, queue_size) == -1)
    {
        throw std::runtime_error("[listen]: " + std::string(strerror(errno)));
    }
    std::cout << "Listening on port " << ntohs(_server_address.sin_port) << "\n\n";
    _accept_thread = std::thread(&Server::accept_clients, this);

    for(;;)
    {
        std::string input;
        std::getline(std::cin, input);

        if(input == "exit")
        {
            return;
        }
        input = "[Server] " + input;
        broadcast_message(input);  
    }
}

void Server::accept_clients()
{
    for(;;)
    {
        struct sockaddr_in client_address = {};
        socklen_t socket_size;

        int client_socket = accept(_server_socket, reinterpret_cast<sockaddr*>(&client_address), &socket_size);
        if (client_socket == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }
            else
            {
                return;
            }
        }
        _clients[client_socket] = std::make_unique<ClientHandler>(client_socket, client_address, this, _ssl_manager);
        std::cout << "Client Connected: [IP: " << _clients[client_socket]->get_ip_str() << "]" << '\n';
    }
}

void Server::broadcast_message(const std::string& message)
{
    for (auto& client: _clients)
    {
        client.second->send_message(message);
    }    
}

void Server::disconnect(int client_socket)
{
    std::cout << "Client Disconnect: [IP: " << _clients[client_socket]->get_ip_str() << "]" << '\n';
    _clients.erase(client_socket);
}


Server::~Server()
{
    shutdown(_server_socket, SHUT_RD);
    _accept_thread.join();
    broadcast_message("[Server] The server is closed.");
    close(_server_socket);
}
