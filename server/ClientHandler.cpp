#include "ClientHandler.h"

ClientHandler::ClientHandler(int socket, struct sockaddr_in address, IServer* server, SSLManager* ssl_manager):
_client_socket(socket), _client_address(address), _server(server), _ssl_manager(ssl_manager)
{
    _ssl = _ssl_manager->create_ssl(socket);
    _ssl_manager->perform_handshake(_ssl);

    _client_thread = std::thread(&ClientHandler::handle, this);
    _client_thread.detach();
    //공유자원 활용, 핸들부분이 서버쪽 cpp로, 아큐먼트로 클라이언트 목록 공유
}

void ClientHandler::handle()
{
    for(;;)
    {
        char buffer[4096];
        int bytes_received = SSL_read(_ssl, buffer, sizeof(buffer) - 1);
        if (bytes_received <= 0)
        {
            break;
        }
        buffer[bytes_received] = '\0';

        std::string message = buffer;
        std::string brodcast_filter = "broadcast ";
        if (message.find(brodcast_filter) == 0)
        {
            message = "[" + get_ip_str() + "] " + message.substr(brodcast_filter.size());
            _server->broadcast_message(message);
        }
        else
        {
            send_message(message);
        }
    }
    _server->disconnect(_client_socket);
}

void ClientHandler::send_message(const std::string& message)
{
    if (SSL_write(_ssl, message.c_str(), message.size()) <= 0)
    {
        throw std::runtime_error("[send]: " + std::string(strerror(errno)));
    }
}

std::string ClientHandler::get_ip_str()
{
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(_client_address.sin_addr), ip_str, INET_ADDRSTRLEN);
    return std::string(ip_str);
}

ClientHandler::~ClientHandler()
{
    _ssl_manager->shutdown_ssl(_ssl);
    close(_client_socket);
}