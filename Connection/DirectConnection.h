
#ifndef PROXY_DIRECTCONNECTION_H
#define PROXY_DIRECTCONNECTION_H


#include <netinet/in.h>
#include "Connection.h"

class DirectConnection : public Connection {
protected:
    bool connected;
    int forwarding_socket_;
    char buf_cf_[MAX_SEND_SIZE];
    ssize_t data_c_f_;
    ssize_t cfoffset;
    sockaddr_in *serveraddr_;

    void connect();

    ssize_t recvfc(const fd_set &rdfds);

    void sendcf(const fd_set &wrfds);

public:
    virtual void fill_fd_set(fd_set &rdfds, fd_set &wrfds);

    DirectConnection(int client_socket, int forwarding_socket, std::vector<char>& buf_cf, sockaddr_in *serveraddr);

    virtual void exchange_data(const fd_set &rdfds, const fd_set &wrfds);

    virtual ~DirectConnection();
};


#endif

