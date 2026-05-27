#pragma once
// PauseScene — оверлей паузы поверх PlayScene.
// blocksUpdate=true → PlayScene замерзает; blocksDraw=false → видна под нами.
//
// Управление: Up/Down — выбор, Confirm — взять, Esc — Resume.
// Опции: Resume (Pop), Exit to Menu (ставит флаг + Pop, см. PauseSceneContext).

#include "TypedScene.h"
#include "PauseSceneContext.h"
#include <memory>

class PauseScene : public TypedScene<PauseSceneContext>
{
public:
    explicit PauseScene(std::unique_ptr<PauseSceneContext> ctx);

    bool blocksUpdate() const override { return true; }
    bool blocksDraw()   const override { return false; }

    void handleInputT(PauseSceneContext& ctx) override;
    Transition updateT(PauseSceneContext& ctx, float dt) override;
    void drawT(PauseSceneContext& ctx) const override;

private:
    std::unique_ptr<PauseSceneContext> ownedCtx_;
    int  focused_  = 0;
    bool wantPick_ = false;
    bool wantResume_ = false;
};
