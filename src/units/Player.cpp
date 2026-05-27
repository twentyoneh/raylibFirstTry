#include "Player.h"
#include "../util/Log.h"
#include "../progression/XpTable.h"
#include "../abilities/Ability.h"   // полный тип для uptr-деструкции
#include <algorithm>

Player::Player(TextureCache& textures, const std::string& name, Vector2 startPos)
    : GameActor(startPos.x, startPos.y), name_(name)
{
    tex_ = &textures.get("player", "resources/textures/units/player/Player0001.png");

    // Один кадр — место под будущий sprite-sheet.
    sheet_.tex = tex_;
    sheet_.frames.push_back(Rectangle{ 0.f, 0.f, (float)tex_->width, (float)tex_->height });

    Animation idle;
    idle.frames = { 0 };
    idle.fps = 1.f;
    idle.loop = true;
    anim_.add("idle", idle);

    Animation walk;
    walk.frames = { 0 };
    walk.fps = 6.f;
    walk.loop = true;
    anim_.add("walk", walk);

    anim_.play("idle");

    LOG_I("PLAYER", "spawned '%s' at (%.1f, %.1f) hp=%d/%d",
          name_.c_str(), pos_.x, pos_.y, health_, maxHealth_);
}

Player::~Player() = default;

void Player::update(float dt)
{
    anim_.update(dt);
    buffs_.update(dt);
    if (weapon_) {
        weapon_->update(dt * buffs_.multiplier(Buff::Stat::FireRateMul));
    }
    // Способности тикаются из PlayScene (им нужен enemies-список), здесь только
    // внутреннее состояние игрока.
}

void Player::draw() const
{
    if (!tex_) return;

    int frameIdx = anim_.frame();
    Rectangle src = sheet_.frames[(size_t)frameIdx % sheet_.frames.size()];

    Vector2 origin{ src.width * 0.5f, src.height * 0.5f };
    Rectangle dst{ pos_.x, pos_.y, src.width, src.height };
    DrawTexturePro(*tex_, src, dst, origin, 0.f, WHITE);
    DrawCircleLines((int)pos_.x, (int)pos_.y, radius(), LIME);
}

// ---- HP ----

void Player::damage(int dmg)
{
    if (dmg <= 0) return;
    health_ -= dmg;
    LOG_I("PLAYER", "took %d damage, hp=%d/%d", dmg, health_, maxHealth_);
    if (health_ <= 0 && alive_) {
        kill();
        LOG_W("PLAYER", "died");
    }
}

void Player::heal(int amount)
{
    if (amount <= 0) return;
    int before = health_;
    health_ = std::min(maxHealth_, health_ + amount);
    if (health_ != before)
        LOG_I("PLAYER", "healed %d → hp=%d/%d", health_ - before, health_, maxHealth_);
}

void Player::increaseMaxHealth(int by, bool alsoHeal)
{
    maxHealth_ += by;
    if (alsoHeal) health_ += by;
    LOG_I("PLAYER", "max hp +%d → %d/%d", by, health_, maxHealth_);
}

void Player::setMoving(bool moving)
{
    anim_.play(moving ? "walk" : "idle");
}

void Player::equip(std::unique_ptr<Weapon> w)
{
    if (w) LOG_I("PLAYER", "equipped weapon: %s", w->name());
    weapon_ = std::move(w);
}

// ---- XP ----

int Player::xpToNextLevel() const
{
    return XpTable::xpForLevel(level_) - xp_;
}

int Player::gainXp(int amount)
{
    if (amount <= 0) return 0;
    xp_ += amount;
    int levelUps = 0;
    while (xp_ >= XpTable::xpForLevel(level_)) {
        xp_ -= XpTable::xpForLevel(level_);
        level_++;
        levelUps++;
        LOG_I("PLAYER", "LEVEL UP → %d (xp leftover: %d)", level_, xp_);
    }
    return levelUps;
}

// ---- Способности ----

void Player::addAbility(std::unique_ptr<Ability> a)
{
    if (!a) return;
    // Если такая способность уже есть — апгрейдим её, а не плодим дубли.
    for (auto& existing : abilities_) {
        if (std::string(existing->name()) == a->name()) {
            existing->upgrade();
            return;
        }
    }
    LOG_I("PLAYER", "ability added: %s", a->name());
    abilities_.push_back(std::move(a));
}
