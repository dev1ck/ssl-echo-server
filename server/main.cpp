#include <iostream>
#include "Server.h"
#include "SSLManager.h"

#define kPort 8080
#define kBacklogQueueSize 5
#define kCertFile "../ssl/cert.pem"
#define kKeyFile "../ssl/key.pem"

int main()
{
    try
    {
        SSLManager ssl_manager;
        ssl_manager.load_certificates(kCertFile, kKeyFile);

        Server server(kPort, &ssl_manager);
        server.start(kBacklogQueueSize);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}