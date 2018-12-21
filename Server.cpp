#include <sys/select.h>
#include <sys/socket.h>
#include <stdexcept>
#include <list>
#include <iostream>
#include <cerrno>
#include <string>
#include <cstring>
#include <netinet/in.h>
#include <fcntl.h>

#include "Server.h"
#include "Utils.h"

Server::Server(int listen_port) :
        listen_sockfd_(socket(AF_INET, SOCK_STREAM, 0)) {
    if (listen_sockfd_ == -1) {
        throw std::runtime_error(std::string("socket: ") + strerror(errno));
    }
    if (listen_sockfd_ >= FD_SETSIZE) {
        throw std::runtime_error(std::string("Socket number out of range for select"));
    }
    struct sockaddr_in listenaddr = Utils::get_sockaddr(NULL, listen_port);

    if (bind(listen_sockfd_, reinterpret_cast<struct sockaddr *>(&listenaddr), sizeof(listenaddr))) {
        throw std::runtime_error(std::string("bind: ") + strerror(errno));
    }

    if (listen(listen_sockfd_, SOMAXCONN)) {
        throw std::runtime_error(std::string("listen: ") + strerror(errno));
    }
    if (fcntl(listen_sockfd_, F_SETFL, fcntl(listen_sockfd_, F_GETFL, 0) | O_NONBLOCK) == -1) {
        throw std::runtime_error("Can't make server socket nonblock!");
    }
}

int Server::accept() {
    sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);
    int client_sock = ::accept(listen_sockfd_, reinterpret_cast<sockaddr *>(&client_addr), &addrlen);
    if (client_sock < 0) {
        throw std::runtime_error(std::string("accept: ") + strerror(errno));
    }
    return client_sock;
}

int Server::get_listen_sockfd() {
    return listen_sockfd_;
}