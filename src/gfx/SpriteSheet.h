#pragma once
#include "raylib.h"
#include <vector>

struct SpriteSheet {
    const Texture2D* tex = nullptr;      
    std::vector<Rectangle> frames;       
};