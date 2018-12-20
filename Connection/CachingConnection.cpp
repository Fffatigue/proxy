//
// Created by cristina on 18.12.18.
//

#include "CachingConnection.h"

void CachingConnection::fill_fd_set(fd_set &rdfds, fd_set &wrfds) {
    if(data_f_c_ == 0) {
        FD_SET(forwarding_socket_, &rdfds);
    }
    FD_SET(client_socket_, &wrfds);
}

void CachingConnection::exchange_data(const fd_set &rdfds, const fd_set &wrfds) {

}

void CachingConnection::disconnect() {

}
