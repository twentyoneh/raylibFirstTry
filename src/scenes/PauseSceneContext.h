#pragma once
// PauseSceneContext — данные для сцены паузы. Указатели не владеют.
//
// exitToMenuFlag — указывает на bool внутри PlayScene. PauseScene не может
// сама свопнуть стек на меню (это удалит её, но оставит PlayScene под собой).
// Поэтому она ставит флаг и делает Pop; PlayScene в следующий тик увидит
// флаг и сделает swapToMenu сама.

#include "SceneContext.h"
#include "../config/MenuInput.h"

struct PauseSceneContext : SceneContext
{
    MenuInput input{};
    bool*     exitToMenuFlag = nullptr;
};
