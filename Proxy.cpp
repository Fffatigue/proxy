
#include <algorithm>
#include <csignal>
#include <stdexcept>
#include <cstring>
#include <cerrno>
#include <cstdio>
#include "Proxy.h"
#include "Server.h"

Proxy::Proxy(int listen_port) :
        server_(listen_port),
        maxfd_(0) {
    add_fd(server_.get_listen_sockfd());
}

void Proxy::run() {
    signal(SIGPIPE, SIG_IGN);

    while (true) {
        prepare_sets();
        int selected = select(maxfd_ + 1, &rdfds_, &wrfds_, NULL, NULL);
        if (selected < 0) {
            throw std::runtime_error(std::string("select: ") + strerror(errno));
        } else if (selected > 0) {
            process_connections();
        }
    }

}

void Proxy::prepare_sets() {
    FD_ZERO(&rdfds_);
    FD_ZERO(&wrfds_);
    FD_SET(server_.get_listen_sockfd(), &rdfds_);
    connections_handler.fill_fd_set(rdfds_, wrfds_);
}

void Proxy::process_connections() {
    int sock;
    if (FD_ISSET(server_.get_listen_sockfd(), &rdfds_)) {
        sock = server_.accept();
        connections_handler.enqueue_connection(sock);
        add_fd(sock);
    }
    connections_handler.data_exchange(rdfds_, wrfds_);
    add_fd(connections_handler.getmaxfd());
}

bool Proxy::add_fd(int sockfd) {
    maxfd_ = std::max(maxfd_, sockfd);
    return sockfd < FD_SETSIZE;
}

