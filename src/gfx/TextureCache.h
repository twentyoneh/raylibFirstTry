#pragma once
#include "raylib.h"
#include <unordered_map>
#include <string>
#include <stdexcept>


class TextureCache {
public:
    // явна€ инициализаци€/деинициализаци€
    void load(const std::string& id, const std::string& filePath, bool setFilterBilinear = true);
    bool has(const std::string& id) const;
    const Texture2D& get(const std::string& id) const;
    Texture2D& get(const std::string& id);
    void unload(const std::string& id);
    void unloadAll();

private:
    std::unordered_map<std::string, Texture2D> cache_;
};