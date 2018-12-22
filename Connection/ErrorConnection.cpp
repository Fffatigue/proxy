
#include <string>
#include "ErrorConnection.h"

ErrorConnection::ErrorConnection(int clientfd, std::string errormsg)     {
    for (int i = 0; i < errormsg.length(); i++) {
        buf_fc_[i] = errormsg[i];
    }
    client_socket_ = clientfd;
    data_f_c_ = errormsg.length();
}

void ErrorConnection::fill_fd_set(fd_set &rdfds, fd_set &wrfds) {
    if (data_f_c_ != 0) {
        FD_SET(client_socket_, &wrfds);
    }
}

void ErrorConnection::exchange_data(const fd_set &rdfds, const fd_set &wrfds) {
    sendfc(wrfds);
}

ErrorConnection::~ErrorConnection() {}

