
#include <cstdio>
#include <cstdlib>
#include "Cache.h"

int Cache::getState() {
    return state_;
}

int Cache::getCache(char *buf, int offset, int length) {
    if (state_ != CACHED) {
        return -1;
    }
    length = (length + offset < cache_.size()) ? length + offset : cache_.size();
    for (int i = offset; i < length; i++) {
        buf[i - offset] = cache_[i];
    }
    return length - offset;
}

int Cache::putCache(char *buf, int length) {
    if (state_ != CACHING) {
        return state_;
    }
    if (length + cache_.size() > MAX_CACHE_SIZE) {
        state_ = NOCACHEABLE;
        return state_;
    }
    for (int i = 0; i < length; i++) {
        cache_.push_back(buf[i]);
    }
    return state_;
}

void Cache::setCached() {
    state_ = CACHED;
}

Cache::Cache(std::string &path) : state_(CACHING), clientsUsing_(0), path_(path) {};

void Cache::markUsing() {
    clientsUsing_++;
}

void Cache::markNoUsing() {
    clientsUsing_--;
}

bool Cache::isUsing() {
    if (clientsUsing_ < 0) {
        printf("%d\n", clientsUsing_);
        exit(0);
    }
    return !clientsUsing_ == 0;
}

void Cache::drop() {
    state_ = DROPPED;
}

std::string &Cache::getPath() {
    return path_;
}

