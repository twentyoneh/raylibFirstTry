#pragma once
// Ability — базовый класс пассивной способности игрока.
// Способность тикает каждый кадр, может рисовать визуал и взаимодействует
// с врагами через переданный список.
//
// Поток данных: PlayScene::updateAbilities_ вызывает у каждой способности
// update(dt, player, enemies). draw делается в drawWorld_ после врагов,
// чтобы кружочки/эффекты рисовались поверх.
//
// Особенность: способности накапливаются (например, второй PulseAbility
// уменьшает интервал/увеличивает урон). Удобнее — сделать virtual
// upgrade() на способности, которая повышает её уровень.

#include "../units/Enemy.h"
#include "../units/Player.h"
#include "../util/Log.h"
#include "raylib.h"
#include <vector>

class Ability {
public:
    virtual ~Ability() = default;

    virtual void update(float dt, Player& player, std::vector<Enemy>& enemies) = 0;
    virtual void draw(const Player& player) const {}

    // Применить уровень-апгрейд этой способности (если уже есть в инвентаре).
    virtual void upgrade() {}

    virtual const char* name() const = 0;
    int level() const { return level_; }

protected:
    int level_ = 1;
};
