
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
    if (active_) {
        int ret = recvfc(rdfds);
        if (!active_ && isCacheable_) {
            if (ret) {
                cache_->drop();
                isCacheable_ = false;
            } else {
                cache_->setCached();
            }
        }
        if (active_ && isCacheable_) {
            if (cache_->putCache(buf_fc_, data_f_c_) != Cache::CACHING) {
                isCacheable_ = false;
            }
        }
    }
    if (connected_) {
        if (active_) {
            sendfc(wrfds);
            if (!active_) {
                cache_->drop();
            }
        }
        if (active_) {
            sendcf(wrfds);
            if (!active_) {
                cache_->drop();
                isCacheable_ = false;
            }
        }
    }

}

CachingConnection::CachingConnection(int client_socket, int forwarding_socket, std::vector<char> &buf_cf,
                                     sockaddr_in *serveraddr, Cache *cache) : DirectConnection(client_socket,
                                                                                               forwarding_socket,
                                                                                               buf_cf,
                                                                                               serveraddr),
                                                                              cache_(cache), isCacheable_(true) {
    if (!connected_) {
        cache->drop();
        isCacheable_ = false;
    }
}

CachingConnection::~CachingConnection() {
    cache_->markNoUsing();
}

