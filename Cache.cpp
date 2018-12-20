
#include "Cache.h"

int Cache::getState() {
    return state_;
}

int Cache::getCache(char *buf, int offset, int length) {
    if (state_ != CACHED) {
        return -1;
    }
    length = (length + offset < (*cache).size()) ? length + offset : offset;
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
        return state_;
    }
    unsigned long offset = (*cache).size();
    for (int i = 0; i < length; i++) {
        (*cache)[offset + i] = buf[i];
    }
    return state_;
}

void Cache::setCached() {
    state_ = CACHED;
}

Cache::Cache() : state_(CACHING) {
    cache = new std::vector<char>;
}
