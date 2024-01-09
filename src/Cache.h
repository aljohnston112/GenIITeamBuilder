#ifndef GENIITEAMBUILDER_CACHE_H
#define GENIITEAMBUILDER_CACHE_H

#include <iostream>
#include <unordered_map>
#include <mutex>

template<typename Key, typename Value>
class ThreadSafeCache {
private:
    std::unordered_map<Key, Value> cache;
    std::mutex mutex;

public:
    void insert(const Key& key, const Value& value) {
        std::lock_guard<std::mutex> lock(mutex);
        cache[key] = value;
    }

    bool find(const Key& key, Value& value) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = cache.find(key);
        if (it != cache.end()) {
            value = it->second;
            return true;
        }
        return false;
    }

    bool remove(const Key& key) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = cache.find(key);
        if (it != cache.end()) {
            cache.erase(it);
            return true;
        }
        return false;
    }

};

#endif //GENIITEAMBUILDER_CACHE_H
