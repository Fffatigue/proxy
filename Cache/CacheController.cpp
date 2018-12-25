
#include <cstdio>
#include <iostream>
#include "CacheController.h"

Cache *CacheController::getCache(std::string path) {
    Cache *cache = NULL;
    CacheMap::iterator cacheEntry = cacheMap_.find(path);
#ifdef DEBUG
    std::cout << "Cache queue size " << cacheQueue_.size() << std::endl;
#endif
    //if we don't have cache
    if (cacheEntry == cacheMap_.end()) {
        if (cacheQueue_.size() >= MAX_CACHE_ENTRIES) {
            //if we can't drop cache
            if (!dropCache()) {
                return NULL;
            }
        }
        cache = new Cache(path);
        cache->markUsing();
        cacheMap_[path] = cacheQueue_.insert(cacheQueue_.end(), cache);
        return cache;
        //else if we have cache
    } else if ((*cacheEntry->second)->getState() == Cache::CACHED) {
        cache = (*cacheEntry->second);
        cache->markUsing();
        return cache;
        //else if cache don't downloaded fully
    } else if ((*cacheEntry->second)->getState() == Cache::DROPPED) {
        delete (*(cacheEntry->second));
        cacheQueue_.erase(cacheEntry->second);
        cache = new Cache(path);
        cache->markUsing();
        cacheMap_[path] = cacheQueue_.insert(cacheQueue_.end(), cache);
        return cache;
    }
    //if no cacheable
    return NULL;
}

bool CacheController::dropCache() {
    for (CacheQueue::iterator it = cacheQueue_.begin(); it != cacheQueue_.end(); it++) {
        if ((*it)->getState() == Cache::DROPPED || !(*it)->isUsing()) {
            cacheMap_.erase(cacheMap_.find((*it)->getPath()));
#ifdef DEBUG
            std::cout << "erased cache " << (*it)->getPath() << std::endl;
#endif
            delete (*it);
            cacheQueue_.erase(it);
            return true;
        }
    }
#ifdef DEBUG
    std::cout << "Can't erase nothing" << std::endl;
#endif
    return false;
}

CacheController::~CacheController() {
    for (CacheQueue::iterator it = cacheQueue_.begin(); it != cacheQueue_.end(); it++) {
        delete(*it);
    }
}

