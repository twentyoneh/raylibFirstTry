#pragma once
// BuffPickup — навешивает на игрока заданный Buff.

#include "Pickup.h"
#include "../units/BuffStack.h"

class BuffPickup : public Pickup {
public:
    BuffPickup(Vector2 pos, Buff b, std::string label, Color color)
        : Pickup(pos), buff_(b), label_(std::move(label)), color_(color) {}

    void onPickup(Player& p) override {
        p.buffs().add(buff_);
        consumed_ = true;
    }

    void draw() const override {
        DrawCircleV(pos_, radius(), color_);
        DrawCircleLines((int)pos_.x, (int)pos_.y, radius(), BLACK);
        int fontSize = 10;
        int tw = MeasureText(label_.c_str(), fontSize);
        DrawText(label_.c_str(), (int)pos_.x - tw/2, (int)pos_.y - 28, fontSize, RAYWHITE);
    }

private:
    Buff        buff_;
    std::string label_;
    Color       color_;
};
