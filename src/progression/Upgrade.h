#pragma once
// UpgradeOption — один пункт меню апгрейда. Описывает, что показать
// игроку (title + description), и что сделать с игроком при выборе.
//
// Поле apply — функция, чтобы UpgradeScene не знала ничего о деталях.
// Все знания о том, "что значит +Damage" заперты в лямбде, которую
// собирает UpgradePool.

#include "../units/Player.h"
#include <functional>
#include <string>

struct UpgradeOption {
    std::string title;
    std::string description;
    Color       color = RAYWHITE;
    std::function<void(Player&)> apply;
};
