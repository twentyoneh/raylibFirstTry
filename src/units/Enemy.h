#pragma once
// Enemy — простой враг. Параметры регулируют волну и тип:
//   hp           — здоровье (по умолчанию 1, обычный моб)
//   speed        — пикс/сек
//   touchDmg     — урон игроку при касании
//   xpValue      — сколько XP даёт за убийство
//   weaponDropChance — вероятность (0..1) выронить оружие при смерти
//   isBoss       — флаг визуала и спецлогики (см. PlayScene::resolveCollisions_)
//
// Босс — тот же Enemy с раздутыми статами и isBoss=true. Если потребуется
// уникальное поведение (фазы, телепорты) — выноси в Boss.h как наследник.

#include "GameActor.h"
#include "../util/Log.h"
#include "raylib.h"
#include "raymath.h"

class Enemy : public GameActor {
public:
    struct Stats {
        int   hp                = 1;
        float speed             = 90.f;
        int   touchDmg          = 10;
        int   xpValue           = 1;
        float weaponDropChance  = 0.05f;
        bool  isBoss            = false;
        float radius            = 12.f;
        Color color             = Color{200, 60, 60, 255};
    };

    Enemy(Vector2 pos, Stats s)
        : GameActor(pos.x, pos.y), s_(s), hp_(s.hp) {}

    float radius()         const override { return s_.radius; }
    int   hp()             const { return hp_; }
    int   touchDamage()    const { return s_.touchDmg; }
    int   xpValue()        const { return s_.xpValue; }
    float weaponDropChance() const { return s_.weaponDropChance; }
    bool  isBoss()         const { return s_.isBoss; }

    void damage(int dmg) {
        hp_ -= dmg;
        if (hp_ <= 0 && alive_) {
            kill();
            LOG_T("ENEMY", "%s killed at (%.1f, %.1f)",
                  s_.isBoss ? "BOSS" : "enemy", pos_.x, pos_.y);
        }
    }

    void chase(Vector2 target, float dt) {
        Vector2 dir = Vector2Subtract(target, pos_);
        if (Vector2LengthSqr(dir) < 0.0001f) return;
        dir = Vector2Normalize(dir);
        move(Vector2Scale(dir, s_.speed * dt));
    }

    void draw() const override {
        DrawCircleV(pos_, s_.radius, s_.color);
        DrawCircleLines((int)pos_.x, (int)pos_.y, s_.radius,
                        s_.isBoss ? GOLD : MAROON);
        if (s_.isBoss) {
            // Полоса HP для босса прямо над ним.
            float pct = (float)hp_ / (float)s_.hp;
            int w = (int)(s_.radius * 2.2f);
            int h = 5;
            int x = (int)(pos_.x - w/2);
            int y = (int)(pos_.y - s_.radius - 12);
            DrawRectangle(x, y, w, h, Color{60,60,60,200});
            DrawRectangle(x, y, (int)(w * pct), h, RED);
        }
    }

    // ---- фабрики стандартных врагов ----

    static Stats normalStats() {
        Stats s;
        s.hp = 1;
        s.speed = 90.f;
        s.touchDmg = 10;
        s.xpValue = 1;
        s.weaponDropChance = 0.05f;
        s.radius = 12.f;
        return s;
    }

    static Stats bossStats(int wave) {
        Stats s;
        s.hp     = 30 + wave * 20;
        s.speed  = 70.f;
        s.touchDmg = 25;
        s.xpValue  = 10 + wave * 2;
        s.weaponDropChance = 1.0f;     // босс всегда роняет оружие
        s.isBoss = true;
        s.radius = 28.f;
        s.color  = Color{160, 30, 30, 255};
        return s;
    }

private:
    Stats s_;
    int   hp_;
};
