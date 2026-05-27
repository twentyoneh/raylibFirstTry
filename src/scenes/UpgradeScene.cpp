#include "UpgradeScene.h"
#include "../util/Log.h"
#include "raylib.h"

UpgradeScene::UpgradeScene(std::unique_ptr<UpgradeSceneContext> ctx)
    : TypedScene<UpgradeSceneContext>(*ctx), ownedCtx_(std::move(ctx))
{
    LOG_I("SCENE", "UpgradeScene opened: %d options", (int)ownedCtx_->options.size());
}

void UpgradeScene::handleInputT(UpgradeSceneContext& ctx)
{
    ctx.input.poll();
    int n = (int)ctx.options.size();
    if (n <= 0) return;

    if (ctx.input.pressed(Action::Right) || ctx.input.pressed(Action::Down))
        focused_ = (focused_ + 1) % n;
    if (ctx.input.pressed(Action::Left)  || ctx.input.pressed(Action::Up))
        focused_ = (focused_ - 1 + n) % n;

    // Прямой выбор цифрой
    if      (IsKeyPressed(KEY_ONE)   && n >= 1) { focused_ = 0; wantPick_ = true; }
    else if (IsKeyPressed(KEY_TWO)   && n >= 2) { focused_ = 1; wantPick_ = true; }
    else if (IsKeyPressed(KEY_THREE) && n >= 3) { focused_ = 2; wantPick_ = true; }

    if (ctx.input.pressed(Action::Confirm)) wantPick_ = true;
}

Transition UpgradeScene::updateT(UpgradeSceneContext& ctx, float /*dt*/)
{
    if (!wantPick_) return Transition::None();
    wantPick_ = false;

    if (focused_ >= 0 && focused_ < (int)ctx.options.size() && ctx.player) {
        const auto& opt = ctx.options[focused_];
        LOG_I("SCENE", "upgrade picked: %s", opt.title.c_str());
        if (opt.apply) opt.apply(*ctx.player);
    }
    return Transition::Pop();
}

void UpgradeScene::drawT(UpgradeSceneContext& ctx) const
{
    // Полупрозрачная подложка поверх замёрзшей игры.
    DrawRectangle(0, 0, ctx.screenW, ctx.screenH, Color{0,0,0,180});

    const char* title = "LEVEL UP — выбери апгрейд";
    int titleSize = 30;
    int tw = MeasureText(title, titleSize);
    DrawText(title, (ctx.screenW - tw)/2, 60, titleSize, RAYWHITE);

    if (ctx.player) {
        const char* hint = TextFormat("Level %d   HP %d/%d",
            ctx.player->level(), ctx.player->health(), ctx.player->maxHealth());
        int hw = MeasureText(hint, 18);
        DrawText(hint, (ctx.screenW - hw)/2, 100, 18, LIGHTGRAY);
    }

    // Карточки апгрейда — равномерно по ширине.
    int n = (int)ctx.options.size();
    if (n == 0) {
        const char* err = "no options (something is wrong)";
        DrawText(err, 40, 180, 20, RED);
        return;
    }

    int cardW = 280, cardH = 280;
    int gap   = 24;
    int totalW = n * cardW + (n - 1) * gap;
    int startX = (ctx.screenW - totalW) / 2;
    int y      = (ctx.screenH - cardH) / 2;

    for (int i = 0; i < n; ++i) {
        int x = startX + i * (cardW + gap);
        bool focused = (i == focused_);

        // Фон карточки
        DrawRectangle(x, y, cardW, cardH, Color{30,30,40,255});
        DrawRectangleLinesEx(Rectangle{(float)x, (float)y, (float)cardW, (float)cardH},
                             focused ? 4 : 2,
                             focused ? YELLOW : DARKGRAY);

        // Цветовая полоса наверху
        DrawRectangle(x, y, cardW, 8, ctx.options[i].color);

        // Заголовок
        const std::string& t = ctx.options[i].title;
        int ts = MeasureText(t.c_str(), 24);
        DrawText(t.c_str(), x + (cardW - ts)/2, y + 30, 24, RAYWHITE);

        // Описание (одной строкой; для многострочного — вызывай DrawTextEx с шрифтом)
        const std::string& d = ctx.options[i].description;
        int ds = MeasureText(d.c_str(), 16);
        DrawText(d.c_str(), x + (cardW - ds)/2, y + 70, 16, LIGHTGRAY);

        // Номер для прямого выбора
        const char* num = TextFormat("[%d]", i + 1);
        int ns = MeasureText(num, 18);
        DrawText(num, x + (cardW - ns)/2, y + cardH - 36, 18, GRAY);
    }

    const char* footer = "← → выбор   Enter — взять   1/2/3 — быстро";
    int fw = MeasureText(footer, 18);
    DrawText(footer, (ctx.screenW - fw)/2, ctx.screenH - 36, 18, LIGHTGRAY);
}
