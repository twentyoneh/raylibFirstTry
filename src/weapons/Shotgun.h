#pragma once
// Shotgun — дробовик: один выстрел = N пуль в конусе.
// Магазин на 6 патронов, медленная перезарядка.

#include "Weapon.h"

class Shotgun : public Weapon {
public:
    Shotgun() : Weapon(makeConfig()) {}
    const char* name() const override { return "Shotgun"; }

protected:
    void firePattern(Vector2 origin, Vector2 dir,
                     std::vector<Bullet>& outBullets,
                     const BulletFactory& factory,
                     float damageBuffMul) override
    {
        for (int i = 0; i < cfg_.pelletsPerShot; ++i) {
            spawnOne(origin, jitter(dir, cfg_.spreadDeg),
                     outBullets, factory, damageBuffMul);
        }
    }

private:
    static WeaponConfig makeConfig() {
        WeaponConfig c{};
        c.ammo            = ProjectileKind::ShotgunPellet;
        c.fireRate        = 1.6f;
        c.pelletsPerShot  = 8;
        c.spreadDeg       = 25.f;
        c.magazine        = 6;
        c.reloadTime      = 1.8f;
        c.damageMul       = 1.f;
        return c;
    }
};
