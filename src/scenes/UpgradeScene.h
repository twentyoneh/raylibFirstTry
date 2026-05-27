#pragma once
// UpgradeScene — сцена выбора апгрейда. Пушится поверх PlayScene
// при level-up. Замораживает обновление PlayScene (blocksUpdate=true)
// и рисуется поверх замёрзшей картинки (blocksDraw=false).
//
// Управление: Left/Right (или A/D) — выбрать карточку, Confirm (Enter/Space/ЛКМ)
// — применить и pop. Можно нажать 1/2/3 для прямого выбора.

#include "TypedScene.h"
#include "UpgradeSceneContext.h"
#include <memory>

class UpgradeScene : public TypedScene<UpgradeSceneContext>
{
public:
    explicit UpgradeScene(std::unique_ptr<UpgradeSceneContext> ctx);

    bool blocksUpdate() const override { return true; }  // замораживает PlayScene
    bool blocksDraw()   const override { return false; } // PlayScene видна как фон

    void handleInputT(UpgradeSceneContext& ctx) override;
    Transition updateT(UpgradeSceneContext& ctx, float dt) override;
    void drawT(UpgradeSceneContext& ctx) const override;

private:
    std::unique_ptr<UpgradeSceneContext> ownedCtx_;
    int  focused_   = 0;
    bool wantPick_  = false;
};
