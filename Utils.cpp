#include <sys/socket.h>
#include <memory>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <stdlib.h>
#include <netdb.h>
#include <cerrno>
#include <stdexcept>
#include <sstream>
#include <cstring>
#include <cstdio>
#include "Utils.h"

int Utils::parse_port(const char* s_port)
{
    char* ret;
    errno = 0;
    int port = strtol(s_port, &ret, 10);
    if (errno != 0) {
        throw std::invalid_argument(std::string("strtol: ") + strerror(errno));
    }
    if (ret[0] != '\0' || port > 65535 || port < 0) {
        throw std::invalid_argument("Invalid port! Port should be between 0 and 65335");
    }
    return port;
}

struct sockaddr_in Utils::get_sockaddr(const char* name, int port)
{
    struct sockaddr_in sock_addr;
    struct addrinfo *addr = NULL;
    memset(&sock_addr, 0, sizeof(sock_addr));

    if (name != NULL) {
        struct addrinfo hints;
        prepare_hints(&hints);
        int code = getaddrinfo(name, NULL, &hints, &addr);
        if (code != 0) {
            throw std::invalid_argument(std::string("getaddrinfo: ") + gai_strerror(code));
        }
        memcpy(&sock_addr, addr->ai_addr, sizeof(struct sockaddr));
        freeaddrinfo(addr);
    } else {
        sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(port);

    return sock_addr;
}

void Utils::prepare_hints(struct addrinfo* hints)
{
    memset(hints, 0, sizeof(addrinfo));
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_protocol = IPPROTO_TCP;
}

std::pair<std::string, int> Utils::parsePath(const char *path, int pathlen) {
    int i;
    int port = 0;
    std::string url;
    bool parse_port = false;
    for (i = 0; i < pathlen; i++) {
        if (parse_port) {
            port *= 10;
            port += path[i] - '0';
        }
        if (path[i] == ':') {
            parse_port = true;
            url = std::string(path + 11, i - 11);
        }
    }
    if (!parse_port) {
        return (std::pair<std::string, int>(std::string(path, i), -1));
    } else {
        return (std::pair<std::string, int>(url, port));
    }
}
