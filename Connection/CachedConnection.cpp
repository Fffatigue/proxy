//
// Created by cristina on 18.12.18.
//

#include <cstdio>
#include "CachedConnection.h"

void CachedConnection::fill_fd_set(fd_set &rdfds, fd_set &wrfds) {
    FD_SET(client_socket_, &wrfds);
}

void CachedConnection::exchange_data(const fd_set &rdfds, const fd_set &wrfds) {
    if (data_f_c_ == 0) {
        fcoffset = 0;
        data_f_c_ = cache_->getCache(buf_fc_, offset, MAX_SEND_SIZE);
        if (data_f_c_ == 0) {
            active = false;
            return;
        }
        offset += data_f_c_;
    }
    sendfc(wrfds);
}

CachedConnection::CachedConnection(int client_socket, Cache *cache) : cache_(cache),
                                                                      offset(0) { client_socket_ = client_socket; }

