#pragma once
// WeaponConfig — параметры стрельбы (что отличает пистолет от дробовика
// или автомата). Сама пуля описывается отдельно: ProjectileKind +
// BulletConfig (см. units/Bullet.h, BulletConfig.h).

#include "../units/Bullet.h"

struct WeaponConfig {
    ProjectileKind ammo         = ProjectileKind::BasicBullet;
    float fireRate              = 5.0f;   // выстрелов в секунду
    int   pelletsPerShot        = 1;      // 1 — обычный, 8 — дробовик
    float spreadDeg             = 0.f;    // ±половина конуса разброса
    int   burstCount            = 1;      // выстрелов в очереди подряд
    float burstDelay            = 0.05f;  // задержка внутри очереди
    int   magazine              = 0;      // 0 = бесконечный, иначе reload
    float reloadTime            = 1.5f;
    float damageMul             = 1.f;    // множитель урона поверх BulletConfig
};
