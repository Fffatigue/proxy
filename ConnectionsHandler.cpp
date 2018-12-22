
#include <string>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <list>
#include <stdexcept>
#include <iostream>
#include "third-party/picohttpparser.h"
#include "ConnectionsHandler.h"
#include "Connection/DirectConnection.h"
#include "Utils.h"
#include "Connection/ErrorConnection.h"
#include "Connection/CachingConnection.h"
#include "Connection/CachedConnection.h"

#define DEBUG

void ConnectionsHandler::process_enqueued(fd_set &rdfds) {
    for (std::map<int, std::vector<char> >::iterator i = _queue.begin(); i != _queue.end();) {
        if (FD_ISSET(i->first, &rdfds)) {
            read_request(i++);
        } else {
            ++i;
        }
    }
}

void ConnectionsHandler::enqueue_connection(int client_sock) {
    _queue[client_sock] = std::vector<char>();
}

int ConnectionsHandler::getmaxfd() {
    return _maxfd;
}

void ConnectionsHandler::fill_fd_set(fd_set &rdfds, fd_set &wrfds) {
    for (std::map<int, std::vector<char> >::iterator i = _queue.begin(); i != _queue.end(); ++i) {
        FD_SET(i->first, &rdfds);
    }
    for (std::list<Connection *>::iterator i = _connections.begin(); i != _connections.end();) {
        if ((*i)->isActive()) {
            (*i)->fill_fd_set(rdfds, wrfds);
        } else {
            delete (*i);
            _connections.erase(i++);
            continue;
        }
        ++i;
    }
    // std::cout << "connections.size(): " << _connections.size() << std::endl;
    // std::cout <<"queue.size(): "<<_queue.size()<<std::endl;
}


void ConnectionsHandler::data_exchange(fd_set &rdfds, fd_set &wrfds) {
    process_enqueued(rdfds);
    for (std::list<Connection *>::iterator i = _connections.begin(); i != _connections.end(); ++i) {
        (*i)->exchange_data(rdfds, wrfds);
    }
}

void ConnectionsHandler::add_connection(std::map<int, std::vector<char> >::iterator client, size_t endpos) {
    int client_sock = client->first;
    const char *path;
    const char *method;
    int minor_version;
    struct phr_header headers[100];
    size_t prevbuflen = 0, method_len, path_len, num_headers;
    num_headers = sizeof(headers) / sizeof(headers[0]);
    char oldRequest[endpos];
    std::vector<char> newRequest;
    for (int i = 0; i < endpos; i++) {
        oldRequest[i] = client->second[i];
    }
    phr_parse_request(oldRequest, endpos, &method, &method_len, &path, &path_len,
                      &minor_version, headers, &num_headers, prevbuflen);/*
#ifdef DEBUG
    printf("method is %.*s\n", (int) method_len, method);
    printf("path is %.*s\n", (int) path_len, path);
    printf("HTTP version is 1.%d\n", minor_version);
    for (int i = 0; i != num_headers; ++i) {
        printf("%.*s: %.*s\n", (int) headers[i].name_len, headers[i].name,
               (int) headers[i].value_len, headers[i].value);
    }

#endif*/
    //if not http 1.0
    if (minor_version != 0) {
        _connections.push_back(new ErrorConnection(client_sock, "HTTP/1.0 505 HTTP Version Not Supported\r\n\r\n"));
        _queue.erase(client);
        return;
    }
    //if method GET
    if (!strncmp(method, "GET", 3)) {
        Utils::makeNewRequest(oldRequest,newRequest,headers,num_headers);
        std::pair<std::string, int> url_port;
        for (int i = 0; i < num_headers; i++) {
            if (!strncmp(headers[i].name, "Host", headers[i].name_len < 4 ? headers[i].name_len : 4)) {
                url_port = Utils::parsePath(headers[i].value, headers[i].value_len);
                break;
            }
            //TODO persistent connection
            //TODO no host error
        }
        //TODO add erase if we don't find host param and send error
        Cache *cache = cacheController_.getCache(std::string(path, path_len));

        //if cache now unavailable
        if (cache == NULL || cache->getState() == Cache::CACHING || cache->getState() == Cache::NOCACHEABLE) {
            int forwarding_sock = socket(AF_INET, SOCK_STREAM, 0);
            int forward_port = url_port.second == -1 ? 80 : url_port.second;
            sockaddr_in forwaddr;
            try {
                forwaddr = Utils::get_sockaddr(url_port.first.c_str(), forward_port);
            } catch (std::invalid_argument &e) {
                _queue.erase(client);
                close(client_sock);
                close(forwarding_sock);
                return;
            }
            _maxfd = std::max(_maxfd, forwarding_sock);
            //if cache now caching or nocacheable
            if (cache == NULL || cache->getState() == Cache::NOCACHEABLE) {
#ifdef DEBUG
                printf("Direct connection to %.*s\n", (int) path_len, path);
#endif
                _connections.push_back(new DirectConnection(client_sock, forwarding_sock, newRequest,
                                                            &forwaddr));
            } else {
#ifdef DEBUG
                printf("Caching connection to  %.*s\n", (int) path_len, path);
#endif
                _connections.push_back(
                        new CachingConnection(client_sock, forwarding_sock, newRequest, &forwaddr, cache));
            }
            _queue.erase(client);
            return;
        } else {
            _connections.push_back(new CachedConnection(client_sock, cache));
#ifdef DEBUG
            printf("Cached connection to %.*s\n", (int) path_len, path);
#endif
        }
    } else {
#ifdef DEBUG
        printf("Error connection to %.*s\n", (int) path_len, path);
#endif
        _connections.push_back(new ErrorConnection(client_sock, "HTTP/1.0 501 Not Implemented\r\n\r\n"));
    }
    _queue.erase(client);
}

void ConnectionsHandler::read_request(std::map<int, std::vector<char> >::iterator client) {
    size_t endpos = -1;
    char buf[MAX_REQUEST_SIZE];
    ssize_t len = read(client->first, buf, MAX_REQUEST_SIZE);
    //TODO MAX BUF SIZE
    for (ssize_t i = 0; i < len; i++) {
        if (i < len - 3) {
            if (buf[i] == '\r' && buf[i + 2] == '\r' && buf[i + 3] == '\n' && buf[i + 1] == '\n') {
                endpos = i + 3;
            }
        }
        client->second.push_back(buf[i]);
    }
    if (endpos != -1) {
        add_connection(client, endpos);
    }
}

