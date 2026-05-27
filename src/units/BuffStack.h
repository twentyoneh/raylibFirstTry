#pragma once
// Buff/BuffStack — лёгкая система модификаторов характеристик.
//
// Модель:
//  - Buff содержит, какую характеристику модифицировать (stat), значение и
//    длительность (0 = постоянный).
//  - BuffStack хранит активные бафы, тикает их время и считает итоговый
//    множитель/добавку по требованию.
//  - При расчёте урона: damage * stack.multiplier(DamageMul).
//  - При расчёте скорости: speed * stack.multiplier(SpeedMul).
//
// Расширение: добавь значение в enum Stat, новый pickup → BuffStack::add(...).

#include "../util/Log.h"
#include <vector>
#include <algorithm>

struct Buff {
    enum class Stat {
        DamageMul,   // множитель урона
        FireRateMul, // множитель скорострельности
        SpeedMul,    // множитель скорости движения
        MaxHpAdd     // добавка к максимальному HP (additive)
    };
    Stat  stat       = Stat::DamageMul;
    float value      = 1.f;     // для Mul-статов: 1.0 — нейтрально; для Add — прибавляется
    float duration   = 0.f;     // 0 = постоянный
    float remaining  = 0.f;     // сколько осталось до истечения
};

class BuffStack {
public:
    void add(Buff b) {
        b.remaining = b.duration;
        buffs_.push_back(b);
        LOG_I("BUFF", "added: stat=%d value=%.2f duration=%.2f",
              (int)b.stat, b.value, b.duration);
    }

    void update(float dt) {
        // Декрементим время у временных, постоянные (duration<=0) не трогаем.
        for (auto& b : buffs_) {
            if (b.duration > 0.f) b.remaining -= dt;
        }
        buffs_.erase(
            std::remove_if(buffs_.begin(), buffs_.end(),
                [](const Buff& b){ return b.duration > 0.f && b.remaining <= 0.f; }),
            buffs_.end());
    }

    // Перемножает все Mul-бафы данного стата (для нейтрального — 1).
    float multiplier(Buff::Stat s) const {
        float m = 1.f;
        for (const auto& b : buffs_) if (b.stat == s) m *= b.value;
        return m;
    }

    // Складывает Add-бафы.
    float additive(Buff::Stat s) const {
        float a = 0.f;
        for (const auto& b : buffs_) if (b.stat == s) a += b.value;
        return a;
    }

    const std::vector<Buff>& list() const { return buffs_; }

private:
    std::vector<Buff> buffs_;
};
