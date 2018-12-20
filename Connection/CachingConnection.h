//
// Created by cristina on 18.12.18.
//

#ifndef PROXY_CACHINGCONNECTION_H
#define PROXY_CACHINGCONNECTION_H


#include "Connection.h"
#include "DirectConnection.h"

class CachingConnection : public DirectConnection{
private:
    Cache cache_;
public:
    void exchange_data(const fd_set& rdfds, const fd_set& wrfds);

};


#endif //PROXY_CACHINGCONNECTION_H
