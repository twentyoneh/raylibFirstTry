#pragma once
// UpgradeSceneContext — данные для сцены выбора апгрейда.
//  - player: указатель на игрока в PlayScene (UpgradeScene не владеет!)
//  - options: 3 случайных опции из UpgradePool
//  - input: ввод для меню (Up/Down/Left/Right/Confirm)
//
// Создаётся при level-up в PlayScene и передаётся UpgradeScene в
// unique_ptr (см. PlayScene::tryPushUpgrade_).

#include "SceneContext.h"
#include "../config/MenuInput.h"
#include "../progression/Upgrade.h"
#include "../units/Player.h"
#include <vector>

struct UpgradeSceneContext : SceneContext
{
    MenuInput                  input{};
    Player*                    player = nullptr;
    std::vector<UpgradeOption> options;
};
