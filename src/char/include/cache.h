#pragma once

// a basic cache that calls a callback<V(const K&)> with the key when not present. K needs to implement std::hash<K>
// cache invalidation is manual

#include <unordered_map>
#include <functional>

template <typename K, typename V>
class Cache {
  public:
    Cache(std::function<V(const K&)> callback) : callback(callback) {}
    
    const V& get_value(const K& k) {
      if (cache.contains(k)) {
        return cache.at(k);
      }
      return *cache.insert({k, callback(k)}).first;
    }
  
    const V& insert_value(const K& k, V&& v) {
      return *cache.insert({k, std::move(v)}).first;
    }
  
    void insert_value(const std::vector<K>& ks, V&& v) {
      for (auto& k: ks) {
        cache.insert({k, v});
      }
    }
    
    void delete_value(const K& k) {
      cache.erase(k);
    }

  private:
    std::unordered_map<K, V> cache;
};
