#pragma once
// Weapon — абстрактный интерфейс оружия.
//
// Идея: Player держит unique_ptr<Weapon> и не знает деталей. PlayScene
// каждый кадр вызывает weapon->update(dt) и при нажатии огня —
// weapon->tryFire(...). Конкретные классы (BasicGun, Shotgun, ...)
// реализуют паттерн стрельбы и сами решают, готовы ли выстрелить
// (cooldown, перезарядка).
//
// Расширение: новые оружия — это новый класс-наследник + конфиг. Player
// и сцена менять не нужно. Подобрать пикап = заменить unique_ptr.

#include "WeaponConfig.h"
#include "../units/Bullet.h"
#include "../units/BulletFactory.h"
#include "../util/Log.h"
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <string>

class Weapon {
public:
    explicit Weapon(WeaponConfig cfg) : cfg_(cfg), ammoLeft_(cfg.magazine) {}
    virtual ~Weapon() = default;

    // Тикает таймеры (cooldown, burst, reload).
    virtual void update(float dt) {
        if (cooldown_ > 0.f) cooldown_ -= dt;
        if (reloading_) {
            reloadLeft_ -= dt;
            if (reloadLeft_ <= 0.f) {
                ammoLeft_  = cfg_.magazine;
                reloading_ = false;
                LOG_I("WEAPON", "%s reloaded (%d ammo)", name(), ammoLeft_);
            }
        }
    }

    // Попытка выстрелить. Возвращает true, если хотя бы одна пуля
    // вылетела. Пули добавляются в outBullets через factory.make().
    // damageBuffMul — внешний множитель урона (бафы игрока).
    virtual bool tryFire(Vector2 origin, Vector2 dir,
                         std::vector<Bullet>& outBullets,
                         const BulletFactory& factory,
                         float damageBuffMul = 1.f)
    {
        if (cooldown_ > 0.f) return false;
        if (reloading_)      return false;
        if (cfg_.magazine > 0 && ammoLeft_ <= 0) {
            startReload();
            return false;
        }

        firePattern(origin, dir, outBullets, factory, damageBuffMul);

        cooldown_ = 1.f / cfg_.fireRate;
        if (cfg_.magazine > 0 && --ammoLeft_ <= 0) startReload();
        return true;
    }

    virtual const char* name() const = 0;

    const WeaponConfig& config() const { return cfg_; }
    int  ammo() const { return ammoLeft_; }
    bool isReloading() const { return reloading_; }
    float reloadProgress() const {
        return cfg_.reloadTime > 0.f ? 1.f - reloadLeft_ / cfg_.reloadTime : 1.f;
    }

protected:
    // Сам паттерн (где и куда летят пули). Базовая реализация — один
    // выстрел по направлению. Наследники меняют под свой стиль.
    virtual void firePattern(Vector2 origin, Vector2 dir,
                             std::vector<Bullet>& outBullets,
                             const BulletFactory& factory,
                             float damageBuffMul)
    {
        spawnOne(origin, dir, outBullets, factory, damageBuffMul);
    }

    // Утилита: создать одну пулю с учётом множителя урона.
    void spawnOne(Vector2 origin, Vector2 dir,
                  std::vector<Bullet>& outBullets,
                  const BulletFactory& factory,
                  float damageBuffMul) const
    {
        Bullet b = factory.make(origin, dir, cfg_.ammo);
        b.damage = (int)((float)b.damage * cfg_.damageMul * damageBuffMul);
        outBullets.push_back(b);
    }

    // Случайный поворот вектора в пределах ±spreadDeg/2 градусов.
    static Vector2 jitter(Vector2 dir, float spreadDeg) {
        if (spreadDeg <= 0.f) return dir;
        float r = ((float)GetRandomValue(-1000, 1000) / 1000.f) * (spreadDeg * 0.5f);
        float rad = r * DEG2RAD;
        float c = cosf(rad), s = sinf(rad);
        return Vector2{ dir.x * c - dir.y * s, dir.x * s + dir.y * c };
    }

    void startReload() {
        if (cfg_.magazine <= 0) return;
        reloading_  = true;
        reloadLeft_ = cfg_.reloadTime;
        LOG_I("WEAPON", "%s reloading...", name());
    }

    WeaponConfig cfg_;
    float cooldown_   = 0.f;
    int   ammoLeft_   = 0;       // инициализируется в конструкторе из cfg.magazine
    bool  reloading_  = false;
    float reloadLeft_ = 0.f;
};
