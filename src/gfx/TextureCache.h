#pragma once
#include "raylib.h"
#include <unordered_map>
#include <string>


class TextureCache {
public:

	~TextureCache() { clear(); }

    const Texture2D& get(const std::string& key, const std::string& path) {
        auto it = map_.find(key);
        if (it != map_.end()) return it->second;
        Texture2D tex = LoadTexture(path.c_str());
        map_.emplace(key, tex);
        return map_.at(key);
    }

    // если где-то нужно явно освободить одну
    void unload(const std::string& key) {
        auto it = map_.find(key);
        if (it != map_.end()) {
            UnloadTexture(it->second);
            map_.erase(it);
        }
    }

private:

    void clear() {
        for (auto& [k, t] : map_) UnloadTexture(t);
        map_.clear();
    }

    std::unordered_map<std::string, Texture2D> map_;
};