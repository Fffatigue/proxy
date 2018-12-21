
#include "Cache.h"

int Cache::getState() {
    return state_;
}

int Cache::getCache(char *buf, int offset, int length) {
    if (state_ != CACHED) {
        return -1;
    }
    length = (length + offset < cache->size()) ? length + offset : cache->size();
    for (int i = offset; i < length; i++) {
        buf[i - offset] = (*cache)[i];
    }
    return length - offset;
}

int Cache::putCache(char *buf, int length) {
    if (state_ != CACHING) {
        return state_;
    }
    if (length + (*cache).size() > MAX_CACHE_SIZE) {
        state_ = NOCACHEABLE;
        delete(cache);
        return state_;
    }
    for (int i = 0; i < length; i++) {
        (*cache).push_back(buf[i]);
    }
    return state_;
}

void Cache::setCached() {
    state_ = CACHED;
}

Cache::Cache() : state_(CACHING), clients_using(1){
    cache = new std::vector<char>;
}

void Cache::mark_using() {
    clients_using++;
}

void Cache::mark_no_using() {
    clients_using--;
}

bool Cache::is_using() {
    return clients_using == 0;
}

void Cache::dropCache() {
    delete(cache);
    state_ = DROPPED;
}
