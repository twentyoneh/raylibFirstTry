#pragma once
// PlaySceneContext — пакет «общих сервисов» для PlayScene:
//  - input        : текущая раскладка кнопок и состояние клавиш для геймплея
//  - bullets      : фабрика конфигов пуль по ProjectileKind
//  - textures     : кеш загруженных текстур (живёт сколько живёт контекст)
//  - dropTable    : таблица дропа по очкам
//
// Контекст создаётся при заходе в PlayScene (см. MenuScene::updateT) и
// принадлежит самой PlayScene через unique_ptr — старая утечка исправлена.

#include "SceneContext.h"
#include "../config/PlayInput.h"
#include "../units/BulletFactory.h"
#include "../gfx/TextureCache.h"
#include "../pickups/DropTable.h"

struct MenuScheneContext;  // forward — нужен только указатель для возврата в меню

struct PlaySceneContext : SceneContext
{
    PlayInput     input{};
    BulletFactory bullets{};
    TextureCache  textures{};
    DropTable     dropTable = DropTable::defaultTable();

    // Указатель на контекст меню — живёт всё время в main.cpp на стеке.
    // Используется при возврате из PlayScene в MenuScene (смерть игрока / Esc).
    MenuScheneContext* menuCtx = nullptr;
};
