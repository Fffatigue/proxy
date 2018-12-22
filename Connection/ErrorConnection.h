
#ifndef PROXY_ERRORCONNECTION_H
#define PROXY_ERRORCONNECTION_H


#include "Connection.h"

class ErrorConnection : public Connection {
public:
    ErrorConnection(int clientfd, std::string errormsg);

    void fill_fd_set(fd_set &rdfds, fd_set &wrfds);

    virtual void exchange_data(const fd_set &rdfds, const fd_set &wrfds);

    virtual ~ErrorConnection();
};


#endif

