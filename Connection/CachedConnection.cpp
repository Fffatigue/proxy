
#include <cstdio>
#include "CachedConnection.h"

void CachedConnection::fill_fd_set(fd_set &rdfds, fd_set &wrfds) {
    FD_SET(client_socket_, &wrfds);
}

void CachedConnection::exchange_data(const fd_set &rdfds, const fd_set &wrfds) {
    if (data_f_c_ == 0) {
        fcoffset_ = 0;
        data_f_c_ = cache_->getCache(buf_fc_, offset_, MAX_SEND_SIZE);
        if (data_f_c_ == 0) {
            active_ = false;
            return;
        }
        offset_ += data_f_c_;
    }
    sendfc(wrfds);
}

CachedConnection::CachedConnection(int client_socket, Cache *cache) : cache_(cache),
                                                                      offset_(0) { client_socket_ = client_socket; }

CachedConnection::~CachedConnection() {
    cache_->markNoUsing();
}


