#pragma once
// RunStats — статистика одного забега. Накапливается в PlayScene и
// показывается на RunSummaryScene. Никакой логики, чистые данные.

struct RunStats {
    float timeAlive = 0.f;
    int   wave      = 1;
    int   kills     = 0;
    int   bossKills = 0;
    int   score     = 0;
    int   level     = 1;
    int   xp        = 0;
};
