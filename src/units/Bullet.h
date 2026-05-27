#pragma once
// Bullet — POD-структура: позиция, скорость (как вектор пикс/сек),
// время жизни и параметры урона.
//
// Жизненный цикл: BulletFactory::make создаёт активную пулю, PlayScene
// каждый кадр интегрирует pos += vel*dt и декрементит ttl. Когда ttl<=0
// или пуля попала — active=false и её вытирают.

#include "raylib.h"
#include <cstdint>

enum class ProjectileKind : uint8_t {
    BasicBullet,
    KnightBasic,
    Heavy,
    Fast,
    ShotgunPellet
};

struct Bullet {
    Vector2 pos{};
    Vector2 vel{};
    float   ttl{ 1.0f };
    float   radius{ 4.0f };
    int     damage{ 1 };
    bool    active{ false };
    ProjectileKind kind{ ProjectileKind::KnightBasic };
};
