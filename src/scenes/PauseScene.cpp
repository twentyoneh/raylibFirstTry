#include "PauseScene.h"
#include "../util/Log.h"
#include "raylib.h"

static const char* kOptions[] = { "Resume", "Exit to Menu" };
static const int   kOptionsN  = (int)(sizeof(kOptions) / sizeof(kOptions[0]));

PauseScene::PauseScene(std::unique_ptr<PauseSceneContext> ctx)
    : TypedScene<PauseSceneContext>(*ctx), ownedCtx_(std::move(ctx))
{
    LOG_I("SCENE", "PauseScene opened");
}

void PauseScene::handleInputT(PauseSceneContext& ctx)
{
    ctx.input.poll();

    if (ctx.input.pressed(Action::Down))
        focused_ = (focused_ + 1) % kOptionsN;
    if (ctx.input.pressed(Action::Up))
        focused_ = (focused_ - 1 + kOptionsN) % kOptionsN;

    if (ctx.input.pressed(Action::Confirm)) wantPick_ = true;

    // Esc снова — мгновенный Resume.
    if (ctx.input.pressed(Action::Back)) wantResume_ = true;
}

Transition PauseScene::updateT(PauseSceneContext& ctx, float /*dt*/)
{
    if (wantResume_) {
        LOG_I("SCENE", "pause: resume");
        return Transition::Pop();
    }
    if (!wantPick_) return Transition::None();
    wantPick_ = false;

    if (focused_ == 0) {                       // Resume
        LOG_I("SCENE", "pause: resume");
        return Transition::Pop();
    } else {                                   // Exit to Menu
        LOG_I("SCENE", "pause: exit to menu");
        if (ctx.exitToMenuFlag) *ctx.exitToMenuFlag = true;
        return Transition::Pop();              // PlayScene увидит флаг
    }
}

void PauseScene::drawT(PauseSceneContext& ctx) const
{
    // Тёмная подложка поверх замёрзшей игры.
    DrawRectangle(0, 0, ctx.screenW, ctx.screenH, Color{0,0,0,180});

    const char* title = "PAUSED";
    int ts = MeasureText(title, 48);
    DrawText(title, (ctx.screenW - ts)/2, 120, 48, RAYWHITE);

    int btnW = 280, btnH = 56, gap = 16;
    int totalH = kOptionsN * btnH + (kOptionsN - 1) * gap;
    int x = (ctx.screenW - btnW) / 2;
    int y = (ctx.screenH - totalH) / 2;

    for (int i = 0; i < kOptionsN; ++i) {
        bool focus = (i == focused_);
        int by = y + i * (btnH + gap);
        DrawRectangle(x, by, btnW, btnH, Color{30,30,40,255});
        DrawRectangleLinesEx(Rectangle{(float)x, (float)by, (float)btnW, (float)btnH},
                             focus ? 4 : 2,
                             focus ? YELLOW : DARKGRAY);
        int tw = MeasureText(kOptions[i], 24);
        DrawText(kOptions[i], x + (btnW - tw)/2, by + (btnH - 24)/2,
                 24, focus ? YELLOW : RAYWHITE);
    }

    const char* hint = "↑↓ — выбор   Enter — взять   Esc — продолжить";
    int hw = MeasureText(hint, 18);
    DrawText(hint, (ctx.screenW - hw)/2, ctx.screenH - 40, 18, LIGHTGRAY);
}
