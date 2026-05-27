#pragma once
// BurstRifle — стреляет очередью из burstCount пуль с задержкой burstDelay.
// tryFire запускает очередь; реальные выстрелы внутри очереди обрабатываются
// в update(). Полезно как пример stateful-оружия.

#include "Weapon.h"

class BurstRifle : public Weapon {
public:
    BurstRifle() : Weapon(makeConfig()) {}
    const char* name() const override { return "BurstRifle"; }

    void update(float dt) override {
        Weapon::update(dt);
        if (burstLeft_ > 0) {
            burstTimer_ -= dt;
            if (burstTimer_ <= 0.f) {
                spawnOne(burstOrigin_, jitter(burstDir_, cfg_.spreadDeg),
                         *outBullets_, *factory_, burstDamageBuff_);
                if (--burstLeft_ > 0) burstTimer_ = cfg_.burstDelay;
            }
        }
    }

protected:
    void firePattern(Vector2 origin, Vector2 dir,
                     std::vector<Bullet>& outBullets,
                     const BulletFactory& factory,
                     float damageBuffMul) override
    {
        // Первая пуля летит сразу, остальные ставим в очередь.
        spawnOne(origin, jitter(dir, cfg_.spreadDeg),
                 outBullets, factory, damageBuffMul);

        burstLeft_       = cfg_.burstCount - 1;
        burstTimer_      = cfg_.burstDelay;
        burstOrigin_     = origin;
        burstDir_        = dir;
        outBullets_      = &outBullets;
        factory_         = &factory;
        burstDamageBuff_ = damageBuffMul;
    }

private:
    static WeaponConfig makeConfig() {
        WeaponConfig c{};
        c.ammo        = ProjectileKind::Fast;
        c.fireRate    = 2.5f;
        c.burstCount  = 3;
        c.burstDelay  = 0.07f;
        c.spreadDeg   = 4.f;
        c.magazine    = 30;
        c.reloadTime  = 1.5f;
        return c;
    }

    int   burstLeft_   = 0;
    float burstTimer_  = 0.f;
    Vector2 burstOrigin_{};
    Vector2 burstDir_{};
    std::vector<Bullet>* outBullets_ = nullptr;
    const BulletFactory* factory_    = nullptr;
    float burstDamageBuff_ = 1.f;
};
