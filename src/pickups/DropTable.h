#pragma once
// DropTable — таблица оружий, которые могут выпасть из врагов.
//
// После рефакторинга на XP/уровни здесь остались только оружия.
// Бафы и временные улучшения теперь — UpgradeOption (см. progression/).
//
// Запись: minWave (нижняя граница волны для появления), weight, maker.
// roll(currentWave, pos): фильтрует доступные → взвешенный random.

#include "Pickup.h"
#include "WeaponPickup.h"
#include "../weapons/BasicGun.h"
#include "../weapons/Shotgun.h"
#include "../weapons/BurstRifle.h"
#include "../util/Log.h"
#include <functional>
#include <memory>
#include <vector>

class DropTable {
public:
    using PickupMaker = std::function<std::unique_ptr<Pickup>(Vector2 pos)>;

    struct Entry {
        int         minWave;
        float       weight;
        PickupMaker maker;
        const char* tag;
    };

    void add(Entry e) { entries_.push_back(std::move(e)); }

    std::unique_ptr<Pickup> roll(int currentWave, Vector2 pos) const {
        float total = 0.f;
        for (const auto& e : entries_)
            if (currentWave >= e.minWave) total += e.weight;
        if (total <= 0.f) return nullptr;

        float r = ((float)GetRandomValue(0, 100000) / 100000.f) * total;
        for (const auto& e : entries_) {
            if (currentWave < e.minWave) continue;
            r -= e.weight;
            if (r <= 0.f) {
                LOG_T("DROP", "rolled '%s' at wave=%d", e.tag, currentWave);
                return e.maker(pos);
            }
        }
        return nullptr;
    }

    static DropTable defaultTable() {
        DropTable t;
        t.add({ 1, 3.f,
            [](Vector2 p){ return std::make_unique<WeaponPickup>(
                p, []{ return std::make_unique<BasicGun>(); }, "GUN", LIGHTGRAY); },
            "weapon_basic" });
        t.add({ 2, 2.f,
            [](Vector2 p){ return std::make_unique<WeaponPickup>(
                p, []{ return std::make_unique<Shotgun>(); }, "SHOTGUN", PURPLE); },
            "weapon_shotgun" });
        t.add({ 3, 1.f,
            [](Vector2 p){ return std::make_unique<WeaponPickup>(
                p, []{ return std::make_unique<BurstRifle>(); }, "BURST", GOLD); },
            "weapon_burst" });
        return t;
    }

private:
    std::vector<Entry> entries_;
};
