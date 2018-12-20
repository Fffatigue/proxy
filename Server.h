
#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H


class Server {
private:
    void process_connections();

    int listen_sockfd_;
public:
    explicit Server(int listen_port);

    int accept();

    int get_listen_sockfd();
};


#endif
