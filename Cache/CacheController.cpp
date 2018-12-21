
#include "CacheController.h"

Cache *CacheController::getCache(std::string path) {
    Cache* cache = NULL;
    CacheMap::iterator cacheEntry = cacheMap_.find(path);
    if (cacheEntry == cacheMap_.end()) {
        cache = new Cache();
        cacheMap_[path] = cache;
        return cache;
    } else if (cacheEntry->second->getState() == Cache::CACHED) {
        cache = cacheEntry->second;
        cache->is_using();
        return cache;
    } else if(cacheEntry->second->getState() == Cache::DROPPED){//TODO delete
        cache = new Cache();
        cacheMap_[path] = cache;
        return cache;
    }
    return NULL;
}
