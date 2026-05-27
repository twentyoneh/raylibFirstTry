// main.cpp — точка входа.
// Архитектура верхнего уровня:
//   InitWindow  → создаём окно raylib
//   Контекст меню MenuScheneContext живёт всё время в стеке main.
//   SceneStack — стек сцен (Menu, Play, ...). См. scenes/SceneStack.h.
//   В цикле:
//     handleInput()  → опрос ввода во все «активные» сцены
//     update(dt)     → шаг логики; верхняя сцена может вернуть Transition
//     draw()         → рендер слоёв снизу вверх
//   Закрываем окно по Esc или Transition::Exit.
//
// Логи: уровень задаётся SetTraceLogLevel. В отладочной сборке полезен DEBUG.

#include "raylib.h"
#include "scenes/MenuScene.h"
#include "scenes/SceneStack.h"
#include "scenes/MenuSceneContext.h"
#include "util/Log.h"

int main(void)
{
    // Чем ниже уровень — тем больше сообщений. LOG_TRACE покажет даже
    // спавн каждого врага. Для тишины поставь LOG_WARNING.
    SetTraceLogLevel(LOG_DEBUG);

    InitWindow(1280, 720, "raylibFirstTry — top-down");
    SetTargetFPS(60);

    MenuScheneContext ctx;
    ctx.screenW = GetScreenWidth();
    ctx.screenH = GetScreenHeight();

    SceneStack stack(ctx);
    stack.push(std::make_unique<MenuScene>(ctx));
    LOG_I("APP", "started; window %dx%d", ctx.screenW, ctx.screenH);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        stack.handleInput();
        stack.update(dt);

        if (stack.wantsExit()) {
            LOG_I("APP", "scene stack requested exit");
            break;
        }

        BeginDrawing();
        stack.draw();
        EndDrawing();
    }

    LOG_I("APP", "shutting down");
    CloseWindow();
    return 0;
}
