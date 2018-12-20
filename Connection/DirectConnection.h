//
// Created by cristina on 18.12.18.
//

#ifndef PROXY_DIRECTCONNECTION_H
#define PROXY_DIRECTCONNECTION_H


#include <netinet/in.h>
#include "Connection.h"

class DirectConnection : public Connection {
protected:
    int forwarding_socket_;
    char buf_cf_[MAX_SEND_SIZE];
    char buf_fc_[MAX_SEND_SIZE];
    int data_c_f_;
    int cfoffset;
    int data_f_c_;
    int fcoffset;
    sockaddr_in *serveraddr_;
    void connect();
    void recvfc(const fd_set &rdfds);
    void sendfc(const fd_set &wrfds);
    void sendcf(const fd_set &wrfds);
public:
    virtual void fill_fd_set(fd_set &rdfds, fd_set &wrfds);
    DirectConnection(int client_socket, int forwarding_socket, std::vector<char> buf_cf, sockaddr_in* serveraddr);

    virtual void exchange_data(const fd_set &rdfds, const fd_set &wrfds);

    virtual ~DirectConnection();
};


#endif //PROXY_DIRECTCONNECTION_H
