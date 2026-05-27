// tests.cpp — мини-набор assert-тестов для чистых функций проекта.
//
// Это НЕ юнит-тестовый фреймворк. Просто <cassert> и стандартный вывод.
// Запуск: build/build-tests.sh (требует уже собранную libraylib.a).
//
// Что покрываем: XpTable, BuffStack, BulletFactory. Это всё функции
// без зависимости от окна/таймера raylib — их безопасно гонять «в воздухе».
//
// Добавление теста:
//  1) static void test_xxx() { assert(...); }
//  2) TEST(test_xxx); в main()
//  Готово.

#include "raylib.h"
#include "progression/XpTable.h"
#include "units/BuffStack.h"
#include "units/Bullet.h"
#include "units/BulletFactory.h"

#include <cassert>
#include <cmath>
#include <iostream>

#define TEST(fn) do {                              \
    std::cout << "  TEST " #fn " ... " << std::flush; \
    fn();                                          \
    std::cout << "OK\n";                           \
} while (0)

// ============================================================
// XpTable
// ============================================================

static void test_xp_strictly_growing() {
    for (int L = 1; L < 50; ++L) {
        assert(XpTable::xpForLevel(L) < XpTable::xpForLevel(L + 1));
    }
}

static void test_xp_known_values() {
    // Формула: 5 + 4*(L-1) + (L-1)^2
    assert(XpTable::xpForLevel(1) == 5);
    assert(XpTable::xpForLevel(2) == 10);
    assert(XpTable::xpForLevel(3) == 17);
    assert(XpTable::xpForLevel(4) == 26);
    assert(XpTable::xpForLevel(5) == 37);
}

static void test_xp_zero_clamped_to_one() {
    // Уровни <1 трактуются как 1 — защита от мусора.
    assert(XpTable::xpForLevel(0) == XpTable::xpForLevel(1));
    assert(XpTable::xpForLevel(-5) == XpTable::xpForLevel(1));
}

// ============================================================
// BuffStack
// ============================================================

static void test_buffs_empty_multiplier_is_one() {
    BuffStack s;
    assert(s.multiplier(Buff::Stat::DamageMul) == 1.0f);
    assert(s.multiplier(Buff::Stat::SpeedMul) == 1.0f);
    assert(s.multiplier(Buff::Stat::FireRateMul) == 1.0f);
}

static void test_buffs_multiplicative_stacking() {
    BuffStack s;
    s.add(Buff{ Buff::Stat::DamageMul, 1.5f, 0.f, 0.f });
    s.add(Buff{ Buff::Stat::DamageMul, 2.0f, 0.f, 0.f });
    // 1.5 * 2.0 = 3.0
    assert(std::fabs(s.multiplier(Buff::Stat::DamageMul) - 3.0f) < 1e-5f);
}

static void test_buffs_different_stats_isolated() {
    BuffStack s;
    s.add(Buff{ Buff::Stat::DamageMul, 2.0f, 0.f, 0.f });
    assert(s.multiplier(Buff::Stat::DamageMul) == 2.0f);
    assert(s.multiplier(Buff::Stat::SpeedMul) == 1.0f);   // не задет
}

static void test_buffs_temporary_expires() {
    BuffStack s;
    s.add(Buff{ Buff::Stat::DamageMul, 2.0f, 1.0f, 0.f });
    assert(std::fabs(s.multiplier(Buff::Stat::DamageMul) - 2.0f) < 1e-5f);
    s.update(0.5f);
    assert(std::fabs(s.multiplier(Buff::Stat::DamageMul) - 2.0f) < 1e-5f);
    s.update(0.6f);   // 1.1 > 1.0 → истёк
    assert(std::fabs(s.multiplier(Buff::Stat::DamageMul) - 1.0f) < 1e-5f);
}

static void test_buffs_permanent_never_expires() {
    BuffStack s;
    s.add(Buff{ Buff::Stat::DamageMul, 2.0f, 0.f, 0.f });   // duration=0 → постоянный
    for (int i = 0; i < 100; ++i) s.update(1.0f);
    assert(std::fabs(s.multiplier(Buff::Stat::DamageMul) - 2.0f) < 1e-5f);
}

static void test_buffs_additive() {
    BuffStack s;
    s.add(Buff{ Buff::Stat::MaxHpAdd, 10.f, 0.f, 0.f });
    s.add(Buff{ Buff::Stat::MaxHpAdd, 15.f, 0.f, 0.f });
    assert(std::fabs(s.additive(Buff::Stat::MaxHpAdd) - 25.f) < 1e-5f);
}

// ============================================================
// BulletFactory
// ============================================================

static float vecLen(Vector2 v) { return std::sqrt(v.x*v.x + v.y*v.y); }

static void test_factory_basic_speed() {
    BulletFactory f;
    Bullet b = f.make(Vector2{100, 200}, Vector2{1, 0},
                      ProjectileKind::BasicBullet);
    assert(b.active);
    assert(b.pos.x == 100.f && b.pos.y == 200.f);
    // BasicBullet: speed=520, direction (1,0) → vel=(520, 0)
    assert(std::fabs(b.vel.x - 520.f) < 1.f);
    assert(std::fabs(b.vel.y) < 1e-3f);
}

static void test_factory_normalizes_direction() {
    BulletFactory f;
    // Направление (3,4) — длина 5, не единичный.
    Bullet b = f.make(Vector2{0, 0}, Vector2{3, 4},
                      ProjectileKind::BasicBullet);
    // Итоговая скорость должна быть равна speed (520), независимо от длины dir.
    float len = vecLen(b.vel);
    assert(std::fabs(len - 520.f) < 1.f);
}

static void test_factory_kinds_differ() {
    BulletFactory f;
    Bullet basic = f.make(Vector2{0,0}, Vector2{1,0}, ProjectileKind::BasicBullet);
    Bullet heavy = f.make(Vector2{0,0}, Vector2{1,0}, ProjectileKind::Heavy);
    assert(basic.damage == 1);
    assert(heavy.damage == 3);
    // Heavy медленнее BasicBullet
    assert(vecLen(heavy.vel) < vecLen(basic.vel));
}

static void test_factory_ttl_radius_from_config() {
    BulletFactory f;
    Bullet b = f.make(Vector2{0,0}, Vector2{1,0}, ProjectileKind::Heavy);
    // Heavy: ttl=2.0, radius=5.0
    assert(std::fabs(b.ttl - 2.0f) < 1e-3f);
    assert(std::fabs(b.radius - 5.0f) < 1e-3f);
}

// ============================================================
// main
// ============================================================

int main() {
    // Глушим INFO-логи внутри BuffStack::add и т.п., чтобы вывод был чистым.
    SetTraceLogLevel(LOG_WARNING);

    std::cout << "Running tests...\n";

    TEST(test_xp_strictly_growing);
    TEST(test_xp_known_values);
    TEST(test_xp_zero_clamped_to_one);

    TEST(test_buffs_empty_multiplier_is_one);
    TEST(test_buffs_multiplicative_stacking);
    TEST(test_buffs_different_stats_isolated);
    TEST(test_buffs_temporary_expires);
    TEST(test_buffs_permanent_never_expires);
    TEST(test_buffs_additive);

    TEST(test_factory_basic_speed);
    TEST(test_factory_normalizes_direction);
    TEST(test_factory_kinds_differ);
    TEST(test_factory_ttl_radius_from_config);

    std::cout << "\nAll tests passed.\n";
    return 0;
}
