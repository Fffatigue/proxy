
#ifndef PROXY_CONNECTIONSHANDLER_H
#define PROXY_CONNECTIONSHANDLER_H

#include <sys/select.h>
#include <vector>
#include <map>
#include "Connection/Connection.h"
#include "Cache/CacheController.h"
#include <list>

class ConnectionsHandler {
    CacheController cacheController_;
    const static int MAX_REQUEST_SIZE = 100000;
    std::list<Connection *> connections_;
    std::map<int, std::vector<char> > queue_;
    int maxfd_;
private:
    void process_enqueued(fd_set &rdfds);

    void read_request(std::map<int, std::vector<char> >::iterator client);

public:
    ConnectionsHandler();

    void enqueue_connection(int client_sock);

    int getmaxfd();

    void fill_fd_set(fd_set &rdfds, fd_set &wrfds);

    void data_exchange(fd_set &rdfds, fd_set &wrfds);

    void add_connection(std::map<int, std::vector<char> >::iterator client, size_t endpos);

};


#endif

