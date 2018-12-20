
#ifndef PROXY_UTILS_H
#define PROXY_UTILS_H


class Utils {
public:
    static int parse_port(const char *s_port);

    static struct sockaddr_in get_sockaddr(const char *name, int port);

    static std::pair<std::string, int> parsePath(const char *path, int pathlen);

private:
    static void prepare_hints(struct addrinfo *hints);
};


#endif
