//
// Created by cristina on 18.12.18.
//

#ifndef PROXY_CACHEDCONNECTION_H
#define PROXY_CACHEDCONNECTION_H


#include "Connection.h"

class CachedConnection : public Connection{
private:
    int offset;
    int client_socket_;
    Cache* cache_;
public:
    CachedConnection(int client_socket,Cache* cache);
    void fill_fd_set(fd_set& rdfds, fd_set& wrfds);
    void exchange_data(const fd_set& rdfds, const fd_set& wrfds);
};


#endif //PROXY_CACHEDCONNECTION_H
