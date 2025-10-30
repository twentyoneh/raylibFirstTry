#include "TextureCache.h"

void TextureCache::load(const std::string& id, const std::string& filePath, bool setFilterBilinear) {
	if (has(id)) return;
	Texture2D texture = LoadTexture(filePath.c_str());
	if (!IsTextureValid(texture)) {
		throw std::runtime_error("Failed to load texture: " + filePath);
	}
	if (setFilterBilinear) SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);	//заменить на нужный scale, если надо увеличить текстуру
	cache_.emplace(id, texture);
}

bool TextureCache::has(const std::string& id) const
{
	return cache_.find(id) != cache_.end();
}

const Texture2D& TextureCache::get(const std::string& id) const
{
	auto it = cache_.find(id);
	if (it == cache_.end()) throw std::runtime_error("Texture not found: " + id);
	return it->second;
}

Texture2D& TextureCache::get(const std::string& id)
{
	auto it = cache_.find(id);
	if (it == cache_.end()) throw std::runtime_error("Texture not found: " + id);
	return it->second;
}

void TextureCache::unload(const std::string& id)
{
	auto it = cache_.find(id);
	if (it != cache_.end()) {
		UnloadTexture(it->second);
		cache_.erase(it);
	}
	else {
		throw std::runtime_error("Texture not found: " + id);
	}
}

void TextureCache::unloadAll()
{
	for (auto& [id, tex] : cache_) {
		UnloadTexture(tex);
	}
	cache_.clear();
}
