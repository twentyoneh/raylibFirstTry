#pragma once
#include "raylib.h"
#include <vector>

struct SpriteSheet {
    const Texture2D* tex = nullptr;      // владеет TextureCache
    std::vector<Rectangle> frames;       // кадры анимаций из атласа
};