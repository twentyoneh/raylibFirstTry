#include "RunSummaryScene.h"
#include "../util/Log.h"
#include "raylib.h"

static const char* kOptions[] = { "Restart", "Exit to Menu" };
static const int   kOptionsN  = (int)(sizeof(kOptions) / sizeof(kOptions[0]));

RunSummaryScene::RunSummaryScene(std::unique_ptr<RunSummarySceneContext> ctx)
    : TypedScene<RunSummarySceneContext>(*ctx), ownedCtx_(std::move(ctx))
{
    LOG_I("SCENE", "RunSummaryScene opened: wave=%d kills=%d time=%.1f",
          ownedCtx_->stats.wave, ownedCtx_->stats.kills, ownedCtx_->stats.timeAlive);
}

void RunSummaryScene::handleInputT(RunSummarySceneContext& ctx)
{
    ctx.input.poll();
    if (ctx.input.pressed(Action::Down) || ctx.input.pressed(Action::Right))
        focused_ = (focused_ + 1) % kOptionsN;
    if (ctx.input.pressed(Action::Up) || ctx.input.pressed(Action::Left))
        focused_ = (focused_ - 1 + kOptionsN) % kOptionsN;
    if (ctx.input.pressed(Action::Confirm)) wantPick_ = true;
}

Transition RunSummaryScene::updateT(RunSummarySceneContext& ctx, float /*dt*/)
{
    if (!wantPick_) return Transition::None();
    wantPick_ = false;

    if (focused_ == 0) {
        LOG_I("SCENE", "summary: restart");
        if (ctx.restartFlag) *ctx.restartFlag = true;
    } else {
        LOG_I("SCENE", "summary: exit to menu");
        if (ctx.exitToMenuFlag) *ctx.exitToMenuFlag = true;
    }
    return Transition::Pop();
}

void RunSummaryScene::drawT(RunSummarySceneContext& ctx) const
{
    DrawRectangle(0, 0, ctx.screenW, ctx.screenH, Color{0,0,0,200});

    const char* title = "GAME OVER";
    int ts = MeasureText(title, 56);
    DrawText(title, (ctx.screenW - ts)/2, 80, 56, RED);

    // Карточка статистики
    int cardW = 440, cardH = 240;
    int cx = (ctx.screenW - cardW) / 2;
    int cy = 170;
    DrawRectangle(cx, cy, cardW, cardH, Color{20,20,28,255});
    DrawRectangleLines(cx, cy, cardW, cardH, GRAY);

    const auto& s = ctx.stats;
    int tx = cx + 32;
    int ty = cy + 24;
    int lh = 28;

    auto line = [&](const char* fmt, auto a, Color col){
        DrawText(TextFormat(fmt, a), tx, ty, 22, col);
        ty += lh;
    };

    DrawText("RESULTS", tx, ty, 24, YELLOW); ty += lh + 6;
    line("Wave reached: %d", s.wave,      RAYWHITE);
    line("Time alive:   %.1f s", s.timeAlive, RAYWHITE);
    line("Kills:        %d", s.kills,     RAYWHITE);
    line("Bosses:       %d", s.bossKills, ORANGE);
    line("Score:        %d", s.score,     SKYBLUE);
    line("Final level:  %d", s.level,     LIME);

    // Кнопки
    int btnW = 220, btnH = 56, gap = 24;
    int totalW = kOptionsN * btnW + (kOptionsN - 1) * gap;
    int bx = (ctx.screenW - totalW) / 2;
    int by = cy + cardH + 36;
    for (int i = 0; i < kOptionsN; ++i) {
        bool focus = (i == focused_);
        int x = bx + i * (btnW + gap);
        DrawRectangle(x, by, btnW, btnH, Color{30,30,40,255});
        DrawRectangleLinesEx(Rectangle{(float)x, (float)by, (float)btnW, (float)btnH},
                             focus ? 4 : 2,
                             focus ? YELLOW : DARKGRAY);
        int tw = MeasureText(kOptions[i], 24);
        DrawText(kOptions[i], x + (btnW - tw)/2, by + (btnH - 24)/2,
                 24, focus ? YELLOW : RAYWHITE);
    }

    const char* hint = "← → — выбор   Enter — взять";
    int hw = MeasureText(hint, 18);
    DrawText(hint, (ctx.screenW - hw)/2, ctx.screenH - 40, 18, LIGHTGRAY);
}
