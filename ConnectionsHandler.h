//
// Created by fffatigue on 19.12.18.
//

#ifndef PROXY_CONNECTIONSHANDLER_H
#define PROXY_CONNECTIONSHANDLER_H

#include <sys/select.h>
#include <vector>
#include <map>
#include "Connection/Connection.h"
#include "Cache/CacheController.h"
#include <list>

class ConnectionsHandler {
private:
    CacheController cacheController_;
    const static int BUF_SIZE = 1000000;
    int _listensockfd;
    std::list<Connection*> _connections;
    std::map<int, std::vector<char> > _queue;
    int _maxfd;
    void process_enqueued(fd_set &rdfds);
    void read_request(std::map<int, std::vector<char> >::iterator client);

public:
    ConnectionsHandler() : _maxfd(0){};
    void enqueue_connection(int client_sock);
    int getmaxfd();
    void fill_fd_set(fd_set& rdfds, fd_set& wrfds);
    void data_exchange(fd_set& rdfds, fd_set& wrfds);
    void add_connection(std::map<int, std::vector<char> >::iterator client, int endpos) ;

};


#endif //PROXY_CONNECTIONSHANDLER_H
