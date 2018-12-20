//
// Created by fffatigue on 20.12.18.
//

#ifndef PROXY_ERRORCONNECTION_H
#define PROXY_ERRORCONNECTION_H


#include "Connection.h"

class ErrorConnection : public Connection{
private:
    const char* buf_fc_;
    size_t data_f_c_;
public:
    ErrorConnection(int clientfd, const char *errormsg);
    void fill_fd_set(fd_set &rdfds, fd_set &wrfds);
    virtual void exchange_data(const fd_set &rdfds, const fd_set &wrfds);
    virtual ~ErrorConnection();
};


#endif //PROXY_ERRORCONNECTION_H
