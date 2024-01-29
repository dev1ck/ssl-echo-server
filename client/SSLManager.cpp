#include "SSLManager.h"

SSLManager::SSLManager()
{
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();

    _ssl_context = SSL_CTX_new(SSLv23_client_method());
    if (not _ssl_context)
    {
        throw std::runtime_error("Unable to create SSL context");
    }
}

void SSLManager::load_certificates(const std::string &cert_file)
{
    if (SSL_CTX_load_verify_locations(_ssl_context, cert_file.c_str(), nullptr) <= 0)
    {
        throw std::runtime_error("Failed to load CA certificates");
    }
}

SSL* SSLManager::create_ssl(int socket)
{
    SSL *ssl = SSL_new(_ssl_context);
    SSL_set_fd(ssl, socket);
    return ssl;
}

void SSLManager::perform_handshake(SSL* ssl)
{
    if (SSL_connect(ssl) <= 0)
    {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("SSL handshake failed");
    }
}

void SSLManager::shutdown_ssl(SSL* ssl)
{
    SSL_shutdown(ssl);
    SSL_free(ssl);
}


SSLManager::~SSLManager()
{
    SSL_CTX_free(_ssl_context);
    EVP_cleanup();
}