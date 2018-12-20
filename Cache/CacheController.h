

#ifndef PROXY_CACHECONTROLLER_H
#define PROXY_CACHECONTROLLER_H


#include <string>
#include <list>
#include <map>
#include <set>
#include "Cache.h"

typedef std::map<std::string, Cache *> CacheMap;

class CacheController {
private:
    CacheMap cacheMap_;
    std::list<CacheMap::iterator> cacheQueue_;
public:
    Cache *getCache(std::string Path);
};


#endif
