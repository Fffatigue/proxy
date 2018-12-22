
#ifndef PROXY_CACHINGCONNECTION_H
#define PROXY_CACHINGCONNECTION_H


#include "Connection.h"
#include "DirectConnection.h"

class CachingConnection : public DirectConnection {
    Cache *cache_;
    bool isCacheable_;
public:
    void fill_fd_set(fd_set &rdfds, fd_set &wrfds);

    void exchange_data(const fd_set &rdfds, const fd_set &wrfds);

    CachingConnection(int client_socket, int forwarding_socket, std::vector<char>& buf_cf, sockaddr_in *serveraddr,
                      Cache *cache);

    virtual ~CachingConnection();
};


#endif

