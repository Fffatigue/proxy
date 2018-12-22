
#ifndef PROXY_PROXY_H
#define PROXY_PROXY_H


#include <sys/select.h>
#include "Server.h"
#include "ConnectionsHandler.h"

class Proxy {
    fd_set rdfds_;
    fd_set wrfds_;
    int listen_sockfd_;
    int maxfd_;
    Server server_;
private:
    ConnectionsHandler connections_handler;

    bool add_fd(int sockfd);

    void prepare_sets();

    void process_connections();

public:
    explicit Proxy(int listen_port);

    void run();
};


#endif

