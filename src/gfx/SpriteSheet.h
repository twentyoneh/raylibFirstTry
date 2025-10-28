#pragma once
#include "raylib.h"
#include <vector>

struct SpriteSheet {
    const Texture2D* tex = nullptr;      // ������� TextureCache
    std::vector<Rectangle> frames;       // ����� �������� �� ������
};