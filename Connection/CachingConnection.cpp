
#include <cstdio>
#include "CachingConnection.h"

void CachingConnection::fill_fd_set(fd_set &rdfds, fd_set &wrfds) {
    if (data_f_c_ == 0) {
        FD_SET(forwarding_socket_, &rdfds);
    }
    if (data_c_f_ != 0) {
        FD_SET(forwarding_socket_, &wrfds);
    }
    FD_SET(client_socket_, &wrfds);
}

void CachingConnection::exchange_data(const fd_set &rdfds, const fd_set &wrfds) {
    if (active) {
        recvfc(rdfds);
        if (!active && isCacheable) {
            cache_->setCached(); //TODO ERRORCHECK
        }
        if (active && isCacheable) {
            if (cache_->putCache(buf_fc_, data_f_c_) != Cache::CACHING) {
                isCacheable = false;
            }
        }
    }
    if (connected) {
        if (active) {
            sendfc(wrfds);
            if (!active) {
                cache_->drop();
            }
        }
        if (active) {
            sendcf(wrfds);
            //TODO drop cache
        }
    }

}

CachingConnection::CachingConnection(int client_socket, int forwarding_socket, std::vector<char>& buf_cf,
                                     sockaddr_in *serveraddr, Cache *cache) : DirectConnection(client_socket,
                                                                                               forwarding_socket,
                                                                                               buf_cf,
                                                                                               serveraddr),
                                                                              cache_(cache), isCacheable(true) {
    if (!connected) {
        //TODO drop
    }
}

CachingConnection::~CachingConnection() {
    cache_->markNoUsing();
}

