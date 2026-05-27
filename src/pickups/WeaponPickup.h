#pragma once
// WeaponPickup — даёт игроку конкретное оружие. Внутри держит фабрику-лямбду,
// чтобы не тащить заголовки конкретных пушек везде, где есть пикапы.

#include "Pickup.h"
#include "../weapons/Weapon.h"
#include <functional>
#include <string>

class WeaponPickup : public Pickup {
public:
    using Maker = std::function<std::unique_ptr<Weapon>()>;

    WeaponPickup(Vector2 pos, Maker maker, std::string label, Color color)
        : Pickup(pos), maker_(std::move(maker)),
          label_(std::move(label)), color_(color) {}

    void onPickup(Player& p) override {
        p.equip(maker_());
        consumed_ = true;
    }

    void draw() const override {
        DrawCircleV(pos_, radius(), color_);
        DrawCircleLines((int)pos_.x, (int)pos_.y, radius(), BLACK);
        // Короткая метка над пикапом, чтобы видеть тип
        int fontSize = 10;
        int tw = MeasureText(label_.c_str(), fontSize);
        DrawText(label_.c_str(), (int)pos_.x - tw/2, (int)pos_.y - 28, fontSize, RAYWHITE);
    }

private:
    Maker        maker_;
    std::string  label_;
    Color        color_;
};
