#pragma once
// RunSummarySceneContext — данные для экрана итогов забега.
// Указатели не владеют. Семантика флагов как у PauseScene:
// сцена сама не свопает; ставит флаг и делает Pop, PlayScene реагирует.

#include "SceneContext.h"
#include "../config/MenuInput.h"
#include "RunStats.h"

struct RunSummarySceneContext : SceneContext
{
    MenuInput input{};
    RunStats  stats{};
    bool*     exitToMenuFlag = nullptr;
    bool*     restartFlag    = nullptr;
};
