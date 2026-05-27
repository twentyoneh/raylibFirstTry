#pragma once
// Player — игрок: позиция, HP, оружие, бафы, опыт, способности.
//
// Прогрессия:
//  - xp_ копится с каждого убитого моба
//  - при xp_ >= xpForLevel(level_) — поднимается уровень и triggerится
//    выбор апгрейда (см. UpgradeScene). xpToNextLevel() возвращает
//    остаток до следующего уровня.
//  - maxHealth_/health_ разделены: апгрейд MaxHP увеличивает maxHealth_.
//
// Способности (Ability): пассивные эффекты, тикают каждый кадр и могут
// бить врагов или баффать игрока. Хранятся как unique_ptr через
// forward declaration, чтобы Player.h не цеплял заголовки конкретных
// способностей.

#include "GameActor.h"
#include "raylib.h"
#include "../gfx/Animator.h"
#include "../gfx/SpriteSheet.h"
#include "BuffStack.h"
#include <string>
#include <memory>
#include <vector>

// Forward declarations: полные типы нужны только в Player.cpp, поэтому
// заголовки этих модулей не тянем в Player.h. Каждый, кто включает
// Player.h, экономит на пре-процессинге их транзитивных зависимостей.
class Ability;
class Weapon;
class TextureCache;

class Player : public GameActor
{
public:
    Player(TextureCache& textures,
           const std::string& name,
           Vector2 startPos);
    ~Player();   // в .cpp — для корректного uptr<Ability>

    void update(float dt) override;
    void draw() const override;

    float radius() const override { return 14.f; }

    const std::string& name() const { return name_; }

    // ---- HP ----
    int  health()    const { return health_; }
    int  maxHealth() const { return maxHealth_; }
    void damage(int dmg);
    void heal(int amount);
    void increaseMaxHealth(int by, bool alsoHeal = true);

    // ---- скорость ----
    float baseSpeed() const { return speed_; }
    void  setSpeed(float s) { speed_ = s; }
    float effectiveSpeed() const { return speed_ * buffs_.multiplier(Buff::Stat::SpeedMul); }

    // ---- Movement state for animator ----
    void setMoving(bool moving);

    // ---- Оружие ----
    Weapon* weapon() { return weapon_.get(); }
    const Weapon* weapon() const { return weapon_.get(); }
    void equip(std::unique_ptr<Weapon> w);

    // ---- Бафы ----
    BuffStack& buffs() { return buffs_; }
    const BuffStack& buffs() const { return buffs_; }

    // ---- Опыт / уровни ----
    int xp()              const { return xp_; }
    int level()           const { return level_; }
    int xpToNextLevel()   const;
    // Принимает XP, возвращает количество поднятых уровней в этом вызове
    // (обычно 0 или 1, но может быть и больше при крупном награждении).
    int gainXp(int amount);

    // ---- Способности ----
    void addAbility(std::unique_ptr<Ability> a);
    const std::vector<std::unique_ptr<Ability>>& abilities() const { return abilities_; }
    std::vector<std::unique_ptr<Ability>>&       abilities()       { return abilities_; }

private:
    std::string name_;
    int   maxHealth_ = 100;
    int   health_    = 100;
    float speed_     = 220.f;       // пикс/сек

    const Texture2D* tex_ = nullptr;
    SpriteSheet sheet_{};
    Animator    anim_{};

    std::unique_ptr<Weapon> weapon_;
    BuffStack buffs_;

    int xp_    = 0;
    int level_ = 1;

    std::vector<std::unique_ptr<Ability>> abilities_;
};
