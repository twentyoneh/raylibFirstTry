#pragma once
#include <raylib.h>
#include <string>
#include "TextureCache.h"
#include <stdexcept>

class Sprite
{
public:
	Sprite() = default;
	void setTexture(TextureCache& cache, const std::string& texId);
    void setSourceRect(Rectangle src);

    void setPosition(Vector2 pos) { position_ = pos; }
    void setScale(Vector2 scale) { scale_ = scale; }
    void setRotation(float deg) { rotation_ = deg; }
    void setOrigin(Vector2 origin) { origin_ = origin; }
    void setTint(Color tint) { tint_ = tint; }

    void setFlip(bool flipX, bool flipY);

    void centerOrigin();

    void draw() const;

    Vector2 position() const { return position_; }
    Vector2 scale() const { return scale_; }
    float   rotation() const { return rotation_; }
    Rectangle sourceRect() const { return srcRect_; }

private:
    const Texture2D* tex_ = nullptr;
    Rectangle srcRect_ = { 0,0,0,0 };

    Vector2 position_ = { 0,0 };
    Vector2 scale_ = { 1,1 };
    float   rotation_ = 0.0f;
    Vector2 origin_ = { 0,0 };
    Color   tint_ = WHITE;
};
