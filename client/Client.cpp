#include "Client.h"

Client::Client(const std::string& ip, int port, SSLManager *ssl_manager) : _ssl_manager(ssl_manager)
{
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == -1)
    {
        throw std::runtime_error("[socket]: " + std::string(strerror(errno)));
    }

    struct in_addr binary_server_ip;
    inet_pton(AF_INET, ip.c_str(), &binary_server_ip);

    _server_address.sin_family = AF_INET;
    _server_address.sin_addr = binary_server_ip;
    _server_address.sin_port = htons(port);

    _ssl = _ssl_manager->create_ssl(_socket);
}

void Client::connect_server()
{
    if (connect(_socket, reinterpret_cast<sockaddr *>(&_server_address), sizeof(_server_address)) == -1)
    {
        throw std::runtime_error("[connect]: " + std::string(strerror(errno)));
    }

    _ssl_manager->perform_handshake(_ssl);
    std::cout << "Connection completed!!" << "\n\n";
}

void::Client::send_message(const std::string& message)
{
    if (SSL_write(_ssl, message.c_str(), message.size()) <= 0)
    {
        throw std::runtime_error("[send]: " + std::string(strerror(errno)));
    }
}

void Client::run()
{
    connect_server();
    _receive_thread= std::thread(&Client::receive_messages, this);

    std::cout << "Send: ";
    for(;;)
    {
        std::string message;
        std::getline(std::cin, message);
        if (message == "exit")
        {
            return;
        }
        send_message(message);
    }
}

void Client::receive_messages()
{
    for(;;)
    {
        char buffer[4096];
        int bytes_received = SSL_read(_ssl, buffer, sizeof(buffer) - 1);
        if (bytes_received <= 0)
        {
           return;
        }
        buffer[bytes_received] = '\0';
        std::cout << '\n' << "Recv: " << buffer << '\n';
        std::cout << "Send: " ;
        std::flush(std::cout);
    }
}

Client::~Client()
{
    shutdown(_socket, SHUT_RD);
    _receive_thread.join();
    _ssl_manager->shutdown_ssl(_ssl);
    close(_socket);
}