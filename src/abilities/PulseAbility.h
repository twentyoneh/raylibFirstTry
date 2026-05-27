#pragma once
// PulseAbility — раз в interval секунд наносит урон всем врагам в радиусе
// вокруг игрока. Визуально — расширяющийся прозрачный круг.
//
// Апгрейд (upgrade()):
//  - уменьшает интервал (быстрее срабатывает)
//  - увеличивает радиус
//  - увеличивает урон
//
// Чтобы добавить ещё одну подобную способность (например, поле молний),
// копируешь этот файл и меняешь логику в pulse_().

#include "Ability.h"
#include "raymath.h"

class PulseAbility : public Ability {
public:
    const char* name() const override { return "Pulse"; }

    void update(float dt, Player& player, std::vector<Enemy>& enemies) override {
        timer_ -= dt;
        if (timer_ <= 0.f) {
            timer_ = interval_;
            pulse_(player, enemies);
        }
        // Затухание визуала.
        if (vfxAlpha_ > 0.f) {
            vfxAlpha_  -= dt * 2.0f;        // полностью гаснет за 0.5 сек
            vfxRadius_ += dt * radius_ * 2.f;
        }
    }

    void draw(const Player& player) const override {
        if (vfxAlpha_ > 0.f) {
            Color c = SKYBLUE;
            c.a = (unsigned char)(vfxAlpha_ * 255.f);
            DrawCircleLines((int)player.position().x, (int)player.position().y,
                            vfxRadius_, c);
            // Внутренняя тонкая заливка для читаемости.
            c.a = (unsigned char)(vfxAlpha_ * 60.f);
            DrawCircleV(player.position(), vfxRadius_, c);
        }
    }

    void upgrade() override {
        level_++;
        damage_  += 1;
        radius_  += 18.f;
        interval_ = std::fmax(0.6f, interval_ - 0.3f);
        LOG_I("ABILITY", "Pulse upgraded to lvl %d (dmg=%d r=%.0f int=%.2f)",
              level_, damage_, radius_, interval_);
    }

    // Параметры (можно подкручивать снаружи через геттеры/сеттеры).
    int   damage()   const { return damage_; }
    float radius()   const { return radius_; }
    float interval() const { return interval_; }

private:
    void pulse_(Player& player, std::vector<Enemy>& enemies) {
        int hits = 0;
        Vector2 c = player.position();
        float r2 = radius_ * radius_;
        for (auto& e : enemies) {
            if (!e.alive()) continue;
            if (Vector2DistanceSqr(e.position(), c) <= r2) {
                e.damage(damage_);
                hits++;
            }
        }
        vfxAlpha_  = 1.f;
        vfxRadius_ = radius_ * 0.4f;
        LOG_T("ABILITY", "Pulse fired: %d hits in r=%.0f", hits, radius_);
    }

    int   damage_   = 1;
    float radius_   = 90.f;
    float interval_ = 3.0f;
    float timer_    = 1.0f;   // первый импульс через 1с после получения

    float vfxAlpha_  = 0.f;
    float vfxRadius_ = 0.f;
};
