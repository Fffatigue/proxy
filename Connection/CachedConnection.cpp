//
// Created by cristina on 18.12.18.
//

#include "CachedConnection.h"

void CachedConnection::fill_fd_set(fd_set &rdfds, fd_set &wrfds) {
    FD_SET(client_socket_, &wrfds);
}

void CachedConnection::exchange_data(const fd_set &rdfds, const fd_set &wrfds) {

}

void CachedConnection::disconnect() {

}
