#include "Client.h"
#include "SSLManager.h"

#include <iostream>
#include <sstream>

#define kCertFile "./cert.pem"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " [ip] [port]" << std::endl;
        return -1;
    }

    std::string str_ip = argv[1];
    std::istringstream iss(argv[2]);
    int port;
    if (not (iss >> port)) {
        std::cerr << "Usage: " << argv[0] << " [ip] [port]" << std::endl;
        return -1;
    }

    try
    {
        SSLManager ssl_manager;
        ssl_manager.load_certificates(kCertFile);

        Client client(str_ip, port, &ssl_manager);
        client.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}