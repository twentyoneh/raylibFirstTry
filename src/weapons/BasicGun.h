#pragma once
// BasicGun — стартовое оружие: одиночный выстрел, средняя скорострельность,
// бесконечный магазин. Только конфиг — паттерн стрельбы из базового класса.

#include "Weapon.h"

class BasicGun : public Weapon {
public:
    BasicGun() : Weapon(makeConfig()) {}
    const char* name() const override { return "BasicGun"; }

private:
    static WeaponConfig makeConfig() {
        WeaponConfig c{};
        c.ammo       = ProjectileKind::BasicBullet;
        c.fireRate   = 6.f;
        c.spreadDeg  = 2.f;
        c.magazine   = 0;
        c.damageMul  = 1.f;
        return c;
    }
};
