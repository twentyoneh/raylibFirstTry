#pragma once
// Pickup — базовый класс «подбираемой штуки» на полу: оружие, баф, аптечка.
// onPickup(player) применяет эффект и pickup помечает себя как использованный.

#include "../units/Player.h"
#include "raylib.h"

class Pickup {
public:
    explicit Pickup(Vector2 pos) : pos_(pos) {}
    virtual ~Pickup() = default;

    Vector2 position() const { return pos_; }
    float   radius()   const { return 14.f; }
    bool    consumed() const { return consumed_; }

    virtual void onPickup(Player& p) = 0;
    virtual void draw() const = 0;

protected:
    Vector2 pos_;
    bool    consumed_ = false;
};
