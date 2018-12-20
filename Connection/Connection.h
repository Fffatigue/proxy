

#ifndef PROXY_CONNECTION_H
#define PROXY_CONNECTION_H

#include <sys/socket.h>
#include "../Cache.h"
#include <unistd.h>

class Connection {
protected:
    const static int MAX_SEND_SIZE = 4096;
    bool active;
    int client_socket_;
public :
    Connection() : active(true) {}

    virtual void fill_fd_set(fd_set &rdfds, fd_set &wrfds)= 0;

    virtual void exchange_data(const fd_set &rdfds, const fd_set &wrfds)= 0;

    bool isActive() {
        return active;
    }

    virtual ~Connection() {
        close(client_socket_);
    }
};


#endif
