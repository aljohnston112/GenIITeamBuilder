#ifndef GENIITEAMBUILDER_THREADSAFECACHE_H
#define GENIITEAMBUILDER_THREADSAFECACHE_H

#include <mutex>
#include <unordered_map>
#include <optional>

template<typename K, typename V>
class ThreadSafeCache {
private:
    std::unordered_map<K, V> cache;
    std::mutex mutex;

public:
    void insert(const K& key, const V& value) {
        std::lock_guard<std::mutex> lock(mutex);
        cache[key] = value;
    }

    bool find(const K& key) {
        std::lock_guard<std::mutex> lock(mutex);

        return cache.find(key) != cache.end();
    }

    std::optional<V> get(const K& key) {
        std::lock_guard<std::mutex> lock(mutex);
        std::optional<V> optional;
        auto it = cache.find(key);
        if (it != cache.end()) {
            optional.emplace(it->second);
        }
        return optional;
    }

    void clear() {
        cache.clear();
    }
};

#endif //GENIITEAMBUILDER_THREADSAFECACHE_H
