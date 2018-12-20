//
// Created by fffatigue on 20.12.18.
//

#include <cstring>
#include "ErrorConnection.h"

ErrorConnection::ErrorConnection(int clientfd, const char *errormsg) : buf_fc_(errormsg) {
    client_socket_ = clientfd;
    data_f_c_ = strlen(errormsg);
}

void ErrorConnection::fill_fd_set(fd_set &rdfds, fd_set &wrfds) {
    if (data_f_c_ != 0) {
        FD_SET(client_socket_, &wrfds);
    }
}

void ErrorConnection::exchange_data(const fd_set &rdfds, const fd_set &wrfds) {
    ssize_t ret;
    if (data_f_c_ != 0 && FD_ISSET(client_socket_, &wrfds)) {
        size_t send_size = data_f_c_ > MAX_SEND_SIZE ? MAX_SEND_SIZE : data_f_c_;
        ret = send(client_socket_, &buf_fc_[0], send_size, 0);
        if (ret == -1) {
            active = false;
            return;
        } else {
            data_f_c_ -= ret;
            //buf_fc_.erase(buf_fc_.begin(), buf_fc_.begin() + ret);
            //TODO add offset
        }
    }
}

ErrorConnection::~ErrorConnection() {}
