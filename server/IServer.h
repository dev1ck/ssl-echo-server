#ifndef I_SERVER_H
#define I_SERVER_H

class IServer {
public:
    virtual void broadcast_message(const std::string& message) = 0;
    virtual void disconnect(int client_socket) = 0;
    virtual ~IServer() {}
};

#endif
