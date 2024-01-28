#include <iostream>
#include "Server.h"
#include "SSLManager.h"

#define kPort 8080
#define kCertFile "./certificate/cert.pem"
#define kKeyFile "./certificate/key.pem"

int main()
{
    try
    {
        SSLManager ssl_manager;
        ssl_manager.load_certificates(kCertFile, kKeyFile);

        Server server(kPort, &ssl_manager);
        server.start(10);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}