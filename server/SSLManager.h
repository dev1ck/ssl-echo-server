#ifndef SSL_MANAGER_H
#define SSL_MANAGER_H

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string>
#include <stdexcept>

class SSLManager
{
private:
    SSL_CTX *_ssl_context;

public:
    SSLManager();
    ~SSLManager();
    void load_certificates(const std::string &cert_file, const std::string &key_file);
    SSL *create_ssl(int socket);
    void perform_handshake(SSL *ssl);
    void shutdown_ssl(SSL *ssl);
};

#endif
