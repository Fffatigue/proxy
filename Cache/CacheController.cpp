
#include "CacheController.h"

Cache *CacheController::getCache(std::string path) {
    Cache* cache = NULL;
    CacheMap::iterator cacheEntry = cacheMap_.find(path);
    if (cacheEntry == cacheMap_.end()) {
        cache = new Cache();
        cacheMap_[path] = cache;
    } else if (cacheEntry->second->getState() == Cache::CACHED) {
        cache = cacheMap_[path];
        cache->is_using();
    } else if(cacheEntry->second->getState() == Cache::DROPPED){
        cache = new Cache();
        cacheMap_[path] = cache;
    }
    return cache;
}
