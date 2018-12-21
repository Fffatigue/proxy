

#ifndef PROXY_CONNECTION_H
#define PROXY_CONNECTION_H

#include <sys/socket.h>
#include "../Cache/Cache.h"
#include <unistd.h>

class Connection {
protected:
    const static int MAX_SEND_SIZE = 4096;
    char buf_fc_[MAX_SEND_SIZE];
    bool active;
    int client_socket_;
    int data_f_c_;
    int fcoffset;

    void sendfc(const fd_set &wrfds) {
        ssize_t ret;
        if (data_f_c_ != 0 && FD_ISSET(client_socket_, &wrfds)) {
            size_t send_size = data_f_c_ > MAX_SEND_SIZE ? MAX_SEND_SIZE : data_f_c_;
            ret = send(client_socket_, &buf_fc_[fcoffset], send_size, 0);
            if (ret == -1) {
                active = false;
                return;
            }
            data_f_c_ -= ret;
            fcoffset += ret;
        }
    }

public :
    Connection() : active(true), client_socket_(), data_f_c_(0), fcoffset(0) {}

    virtual void fill_fd_set(fd_set &rdfds, fd_set &wrfds)= 0;

    virtual void exchange_data(const fd_set &rdfds, const fd_set &wrfds)= 0;

    bool isActive() {
        return active;
    }

    virtual ~Connection() {
        close(client_socket_);
    }
};


#endif
