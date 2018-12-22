
#ifndef PROXY_CONNECTION_H
#define PROXY_CONNECTION_H

#include <sys/socket.h>
#include "../Cache/Cache.h"
#include <unistd.h>
#include <cerrno>

class Connection {
protected:
    const static size_t MAX_SEND_SIZE = 4096;
    char buf_fc_[MAX_SEND_SIZE];
    bool active_;
    int client_socket_;
    ssize_t data_f_c_;
    int fcoffset_;

    void sendfc(const fd_set &wrfds) {
        ssize_t ret;
        if (data_f_c_ != 0 && FD_ISSET(client_socket_, &wrfds)) {
            size_t send_size = data_f_c_ > MAX_SEND_SIZE ? MAX_SEND_SIZE : data_f_c_;
            ret = send(client_socket_, buf_fc_ + fcoffset_, send_size, 0);
            if (ret == -1) {
                if (errno != EWOULDBLOCK) {
                    active_ = false;
                    return;
                }
            }
            data_f_c_ -= ret;
            fcoffset_ += ret;
        }
    }

public :
    Connection() : buf_fc_(), active_(true), client_socket_(), data_f_c_(0), fcoffset_(0) {}

    virtual void fill_fd_set(fd_set &rdfds, fd_set &wrfds) = 0;

    virtual void exchange_data(const fd_set &rdfds, const fd_set &wrfds)= 0;

    bool isActive() {
        return active_;
    }

    virtual ~Connection() {
        close(client_socket_);
    }
};


#endif

