#pragma once
#include "Bullet.h"
#include "BulletConfig.h"
#include <unordered_map>
#include "raymath.h"

class BulletFactory {
public:
    BulletFactory() {
        // Значения по умолчанию
        configs_[ProjectileKind::BasicBullet] = BulletConfig{
            520.f, 1.5f, 4.f, 1, 0 };
        configs_[ProjectileKind::KnightBasic] = BulletConfig{
            520.f, 1.5f, 4.f, 1, 0 };
        configs_[ProjectileKind::Heavy] = BulletConfig{
            360.f, 2.0f, 5.f, 3, 0 };
        configs_[ProjectileKind::Fast] = BulletConfig{
            800.f, 1.0f, 3.f, 1, 0 };
        configs_[ProjectileKind::ShotgunPellet] = BulletConfig{
            620.f, 0.6f, 3.f, 1, 0 };
    }

    const BulletConfig& cfg(ProjectileKind k) const {
        auto it = configs_.find(k);
        return it != configs_.end() ? it->second : configs_.begin()->second;
    }

    // Создание одной пули
    Bullet make(Vector2 origin, Vector2 dir, ProjectileKind k) const {
        Bullet b{};
        b.kind = k;
        b.pos = origin;

        if (dir.x != 0 || dir.y != 0) dir = Vector2Normalize(dir);
        const auto& c = cfg(k);
        b.vel = Vector2Scale(dir, c.speed);
        b.ttl = c.ttl;
        b.radius = c.radius;
        b.damage = c.damage;
        b.active = true;
        return b;
    }

private:
    std::unordered_map<ProjectileKind, BulletConfig> configs_;
};
