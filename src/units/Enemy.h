#pragma once
// Enemy — обобщённый враг. Тип (EnemyKind) определяет поведение и характеристики.
//
// EnemyKind:
//  - Chaser  — стандартный преследователь, hp=1, средняя скорость
//  - Runner  — быстрый и хрупкий, hp=1, скорость ~160, низкий touchDmg
//  - Tank    — толстый и медленный, hp=5, большой radius, большой touchDmg
//  - Shooter — держит дистанцию (desiredDistance) и стреляет (shootInterval)
//  - Boss    — спавнится в конце волны
//
// Поведение в update(): switch по kind. Если потребуется уникальная сложная
// логика (фазы, патруль) — вынеси в отдельный класс-наследник.
//
// Shooter использует EnemyTick.enemyBullets/factory для спавна enemy-пули.
// Pickup'ы пуль и Player видят их отдельным списком в PlayScene.

#include "GameActor.h"
#include "Bullet.h"
#include "BulletFactory.h"
#include "../util/Log.h"
#include "raylib.h"
#include "raymath.h"
#include <vector>

enum class EnemyKind { Chaser, Runner, Tank, Shooter, Boss };

struct EnemyTick {
    Vector2 playerPos{};
    std::vector<Bullet>* enemyBullets = nullptr;
    const BulletFactory* factory      = nullptr;
};

class Enemy : public GameActor {
public:
    struct Stats {
        EnemyKind kind             = EnemyKind::Chaser;
        int   hp                   = 1;
        float speed                = 90.f;
        int   touchDmg             = 10;
        int   xpValue              = 1;
        float weaponDropChance     = 0.05f;
        bool  isBoss               = false;
        float radius               = 12.f;
        Color color                = Color{200, 60, 60, 255};
        // только для Shooter:
        float desiredDistance      = 0.f;   // на какой дистанции стоит/отступает
        float shootInterval        = 0.f;   // сек между выстрелами
    };

    Enemy(Vector2 pos, Stats s)
        : GameActor(pos.x, pos.y), s_(s), hp_(s.hp),
          shootTimer_(s.shootInterval) {}

    float radius()          const override { return s_.radius; }
    int   hp()              const { return hp_; }
    int   touchDamage()     const { return s_.touchDmg; }
    int   xpValue()         const { return s_.xpValue; }
    float weaponDropChance() const { return s_.weaponDropChance; }
    bool  isBoss()          const { return s_.isBoss; }
    EnemyKind kind()        const { return s_.kind; }

    void damage(int dmg) {
        hp_ -= dmg;
        if (hp_ <= 0 && alive_) {
            kill();
            LOG_T("ENEMY", "kind=%d killed at (%.1f, %.1f)",
                  (int)s_.kind, pos_.x, pos_.y);
        }
    }

    // Единственный метод обновления — дальше switch по kind.
    void tick(float dt, const EnemyTick& tick) {
        if (!alive_) return;
        switch (s_.kind) {
            case EnemyKind::Shooter:
                behaviorShooter_(dt, tick); break;
            case EnemyKind::Runner:
            case EnemyKind::Chaser:
            case EnemyKind::Tank:
            case EnemyKind::Boss:
            default:
                behaviorChase_(dt, tick.playerPos); break;
        }
    }

    void draw() const override {
        DrawCircleV(pos_, s_.radius, s_.color);
        DrawCircleLines((int)pos_.x, (int)pos_.y, s_.radius,
                        s_.isBoss ? GOLD : MAROON);
        if (s_.isBoss) {
            // HP-бар над боссом
            float pct = (float)hp_ / (float)s_.hp;
            int w = (int)(s_.radius * 2.2f);
            int h = 5;
            int x = (int)(pos_.x - w/2);
            int y = (int)(pos_.y - s_.radius - 12);
            DrawRectangle(x, y, w, h, Color{60,60,60,200});
            DrawRectangle(x, y, (int)(w * pct), h, RED);
        }
    }

    // ============================================================
    // Фабрики типов
    // ============================================================

    static Stats chaserStats() {
        Stats s;
        s.kind = EnemyKind::Chaser;
        s.hp = 1; s.speed = 90.f; s.touchDmg = 10; s.xpValue = 1;
        s.weaponDropChance = 0.05f; s.radius = 12.f;
        s.color = Color{200, 60, 60, 255};   // red
        return s;
    }
    static Stats runnerStats() {
        Stats s;
        s.kind = EnemyKind::Runner;
        s.hp = 1; s.speed = 170.f; s.touchDmg = 6; s.xpValue = 2;
        s.weaponDropChance = 0.06f; s.radius = 9.f;
        s.color = Color{240, 190, 60, 255};   // yellow/orange
        return s;
    }
    static Stats tankStats() {
        Stats s;
        s.kind = EnemyKind::Tank;
        s.hp = 5; s.speed = 55.f; s.touchDmg = 25; s.xpValue = 4;
        s.weaponDropChance = 0.15f; s.radius = 18.f;
        s.color = Color{120, 120, 140, 255};  // grey
        return s;
    }
    static Stats shooterStats() {
        Stats s;
        s.kind = EnemyKind::Shooter;
        s.hp = 2; s.speed = 75.f; s.touchDmg = 8; s.xpValue = 3;
        s.weaponDropChance = 0.10f; s.radius = 11.f;
        s.color = Color{100, 200, 250, 255};  // cyan
        s.desiredDistance = 260.f;
        s.shootInterval   = 1.6f;
        return s;
    }
    static Stats bossStats(int wave) {
        Stats s;
        s.kind = EnemyKind::Boss;
        s.hp     = 30 + wave * 20;
        s.speed  = 70.f;
        s.touchDmg = 25;
        s.xpValue  = 10 + wave * 2;
        s.weaponDropChance = 1.0f;
        s.isBoss = true;
        s.radius = 28.f;
        s.color  = Color{160, 30, 30, 255};
        return s;
    }

    // Выбор случайного типа обычного моба по волне.
    // На w=1 только Chaser/Runner; со 2 — Tank; с 3 — Shooter.
    static Stats randomForWave(int wave) {
        int r = GetRandomValue(0, 99);
        if (wave >= 3 && r < 15) return shooterStats();
        if (wave >= 2 && r < 25) return tankStats();
        if (wave >= 1 && r < 50) return runnerStats();
        return chaserStats();
    }

private:
    void behaviorChase_(float dt, Vector2 target) {
        Vector2 dir = Vector2Subtract(target, pos_);
        if (Vector2LengthSqr(dir) < 0.0001f) return;
        dir = Vector2Normalize(dir);
        move(Vector2Scale(dir, s_.speed * dt));
    }

    void behaviorShooter_(float dt, const EnemyTick& tick) {
        Vector2 toPlayer = Vector2Subtract(tick.playerPos, pos_);
        float dist = Vector2Length(toPlayer);
        Vector2 dir = (dist > 0.0001f) ? Vector2Scale(toPlayer, 1.f/dist)
                                       : Vector2{1.f, 0.f};

        // Управление дистанцией: ±20 пикс — мёртвая зона.
        if (dist < s_.desiredDistance - 20.f) {
            move(Vector2Scale(dir, -s_.speed * dt));   // отступает
        } else if (dist > s_.desiredDistance + 20.f) {
            move(Vector2Scale(dir,  s_.speed * dt));   // приближается
        }
        // иначе стоит и стреляет

        shootTimer_ -= dt;
        if (shootTimer_ <= 0.f && tick.enemyBullets && tick.factory) {
            shootTimer_ = s_.shootInterval;
            // Стреляет только если игрок в зоне (чуть шире desiredDistance).
            if (dist < s_.desiredDistance + 120.f) {
                Bullet b = tick.factory->make(pos_, dir, ProjectileKind::EnemyShot);
                tick.enemyBullets->push_back(b);
            }
        }
    }

    Stats s_;
    int   hp_;
    float shootTimer_;
};
