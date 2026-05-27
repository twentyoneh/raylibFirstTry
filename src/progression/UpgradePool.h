#pragma once
// UpgradePool — фабрика случайных вариантов апгрейда для UpgradeScene.
// pickRandom(n) возвращает n непохожих опций.
//
// Расширение: добавь новый push_back в фабрики ниже. Можно делать
// опции "разблокировать только если playerLevel >= N" — для этого
// фильтруй на этапе сбора.

#include "Upgrade.h"
#include "../abilities/PulseAbility.h"
#include "raylib.h"
#include <vector>
#include <algorithm>

class UpgradePool {
public:
    static std::vector<UpgradeOption> all() {
        std::vector<UpgradeOption> v;

        // +25 к максимальному HP и сразу heal на ту же величину.
        v.push_back({
            "Max HP +25",
            "Увеличить максимум здоровья",
            GREEN,
            [](Player& p){ p.increaseMaxHealth(25, true); }
        });

        // Лечение до полного, без увеличения максимума.
        v.push_back({
            "Heal full",
            "Восстановить всё здоровье",
            LIME,
            [](Player& p){ p.heal(p.maxHealth()); }
        });

        // Постоянный множитель урона +20%.
        v.push_back({
            "Damage +20%",
            "Постоянный баф урона",
            ORANGE,
            [](Player& p){ p.buffs().add(Buff{Buff::Stat::DamageMul, 1.2f, 0.f, 0.f}); }
        });

        // Постоянный множитель скорости стрельбы +15%.
        v.push_back({
            "Fire rate +15%",
            "Постоянный баф скорострельности",
            YELLOW,
            [](Player& p){ p.buffs().add(Buff{Buff::Stat::FireRateMul, 1.15f, 0.f, 0.f}); }
        });

        // Скорость передвижения +10%.
        v.push_back({
            "Speed +10%",
            "Двигаться быстрее",
            SKYBLUE,
            [](Player& p){ p.buffs().add(Buff{Buff::Stat::SpeedMul, 1.10f, 0.f, 0.f}); }
        });

        // Пульсация — аура урона вокруг игрока. Первый раз добавляет, потом апгрейдит.
        v.push_back({
            "Pulse Aura",
            "Раз в N сек бьёт врагов вокруг",
            PURPLE,
            [](Player& p){ p.addAbility(std::make_unique<PulseAbility>()); }
        });

        return v;
    }

    // Возвращает n уникальных опций (или меньше, если в пуле меньше).
    static std::vector<UpgradeOption> pickRandom(int n) {
        auto pool = all();
        std::vector<UpgradeOption> result;
        for (int i = 0; i < n && !pool.empty(); ++i) {
            int idx = GetRandomValue(0, (int)pool.size() - 1);
            result.push_back(std::move(pool[idx]));
            pool.erase(pool.begin() + idx);
        }
        return result;
    }
};
