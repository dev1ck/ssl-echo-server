#include "ClientHandler.h"

ClientHandler::ClientHandler(int socket, struct sockaddr_in address, IServer* server, SSLManager* ssl_manager):
_client_socket(socket), _client_address(address), _server(server), _ssl_manager(ssl_manager)
{
    _ssl = _ssl_manager->create_ssl(socket);
    _ssl_manager->perform_handshake(_ssl);

    _client_thread = std::thread(&ClientHandler::handle, this);
    _client_thread.detach();
}

void ClientHandler::handle()
{
    while(1)
    {
        char buffer[4096];
        int bytes_received = SSL_read(_ssl, buffer, sizeof(buffer) - 1);

        if (bytes_received <= 0)
        {
            break;
        }
        buffer[bytes_received] = '\0';

        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(_client_address.sin_addr), ip_str, INET_ADDRSTRLEN);
        std::string message = "[" + std::string(ip_str) + "] " + buffer;

        _server->broadcast_message(message);
    }
    stop();
}

void ClientHandler::send_message(const std::string& message)
{
    if (SSL_write(_ssl, message.c_str(), message.size()) <= 0)
    {
        throw std::runtime_error("[send]: " + std::string(strerror(errno)));
    }
}

void ClientHandler::stop()
{
    _ssl_manager->shutdown_ssl(_ssl);
    _server->disconnect(_client_socket);
    close(_client_socket);
}