
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <cstdio>
#include "DirectConnection.h"

void DirectConnection::fill_fd_set(fd_set &rdfds, fd_set &wrfds) {
    if (data_f_c_ == 0) {
        FD_SET(forwarding_socket_, &rdfds);
    }
    if (data_c_f_ != 0) {
        FD_SET(forwarding_socket_, &wrfds);
    }
    FD_SET(client_socket_, &wrfds);
}

void DirectConnection::exchange_data(const fd_set &rdfds, const fd_set &wrfds) {
    if (active) {
        recvfc(rdfds);
    }
    if (active) {
        sendfc(wrfds);
    }
    if (active) {
        sendcf(wrfds);
    }
}


DirectConnection::DirectConnection(int client_socket, int forwarding_socket, std::vector<char> buf_cf,
                                   sockaddr_in *serveraddr) :
        forwarding_socket_(forwarding_socket),
        serveraddr_(serveraddr),
        data_f_c_(0),
        data_c_f_(0), fcoffset(0), cfoffset(0) {
    client_socket_ = client_socket;
    for (int i = 0; i < buf_cf.size(); i++) {
        buf_cf_[i] = buf_cf[i];
    }
    data_c_f_ = buf_cf.size();
    connect();

}

void DirectConnection::connect() {
    int ret;
    for (int numsec = 1; numsec <= 4; numsec *= 2) {
        ret = ::connect(forwarding_socket_, reinterpret_cast<sockaddr *> (serveraddr_), sizeof(*serveraddr_));
        if (ret == 0) {
            return;
        }
        std::cout << "Failed to connect! Retry in " << numsec << " seconds" << std::endl;
        sleep(numsec);
    }
    throw std::runtime_error(std::string("connect: ") + strerror(errno));
}

DirectConnection::~DirectConnection() {
    close(client_socket_);
    close(forwarding_socket_);

}

void DirectConnection::recvfc(const fd_set &rdfds) {
    ssize_t ret;
    if (data_f_c_ == 0 && FD_ISSET(forwarding_socket_, &rdfds)) {
        fcoffset = 0;
        ret = recv(forwarding_socket_, &buf_fc_[0], MAX_SEND_SIZE, 0);
        if (ret == 0 || ret == -1) {
            active = false;
            return;
        }
        data_f_c_ = ret;
    }

}

void DirectConnection::sendfc(const fd_set &wrfds) {
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

void DirectConnection::sendcf(const fd_set &wrfds) {
    ssize_t ret;
    if (data_c_f_ != 0 && FD_ISSET(forwarding_socket_, &wrfds)) {
        size_t send_size = data_c_f_ > MAX_SEND_SIZE ? MAX_SEND_SIZE : data_c_f_;
        ret = send(forwarding_socket_, &buf_cf_[cfoffset], send_size, 0);
        data_c_f_ -= ret;
        cfoffset += ret;
        if (ret == -1) {
            active = false;
            return;
        }
    }
}
