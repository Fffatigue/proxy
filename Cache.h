//
// Created by cristina on 18.12.18.
//

#ifndef PROXY_CACHE_H
#define PROXY_CACHE_H


#include <vector>

class Cache {
private:
    int state_;
    std::vector<char>* cache;
    static const int MAX_CACHE_SIZE = 10000;
public:
    Cache();
    enum STATE{CACHED, CACHING, NOCACHEABLE };
    int getState();
    int getCache(char* buf, int offset, int length);
    int putCache(char* buf, int length);
    void setCached();
};


#endif //PROXY_CACHE_H
