#include "Sprite.h"

void Sprite::setTexture(TextureCache& cache, const std::string& texId) {
    const Texture2D& t = cache.get(texId);
    tex_ = &t;
    // по умолчанию — вся текстура
    srcRect_ = { 0, 0, (float)t.width, (float)t.height };
    // Если флип не нужен — убедимся, что ширина/высота положительные
    if (srcRect_.width < 0)  srcRect_.width = -srcRect_.width;
    if (srcRect_.height < 0) srcRect_.height = -srcRect_.height;
}

void Sprite::setSourceRect(Rectangle src) {
    srcRect_ = src;
}

void Sprite::setFlip(bool flipX, bool flipY) {
    if (!tex_) return;
    // Меняем знак ширины/высоты srcRect для флипа
    if (flipX) {
        srcRect_.x += srcRect_.width;
        srcRect_.width = -srcRect_.width;
    }
    if (flipY) {
        srcRect_.y += srcRect_.height;
        srcRect_.height = -srcRect_.height;
    }
}

void Sprite::centerOrigin() {
    origin_ = { (float)std::abs(srcRect_.width) * 0.5f,
                (float)std::abs(srcRect_.height) * 0.5f };
}

void Sprite::draw() const {
    if (!tex_) return;

    // Размер на экране = srcRect * scale
    Rectangle dst{
        position_.x,
        position_.y,
        (float)std::abs(srcRect_.width) * scale_.x,
        (float)std::abs(srcRect_.height) * scale_.y
    };

    DrawTexturePro(*tex_, srcRect_, dst, origin_, rotation_, tint_);
}