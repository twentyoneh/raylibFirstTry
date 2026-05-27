#pragma once
// RunSummaryScene — показывается при смерти игрока. Замораживает PlayScene
// (blocksUpdate=true) и рисуется поверх (blocksDraw=false), чтобы был виден
// мёртвый герой и арена в момент конца.

#include "TypedScene.h"
#include "RunSummarySceneContext.h"
#include <memory>

class RunSummaryScene : public TypedScene<RunSummarySceneContext>
{
public:
    explicit RunSummaryScene(std::unique_ptr<RunSummarySceneContext> ctx);

    bool blocksUpdate() const override { return true; }
    bool blocksDraw()   const override { return false; }

    void handleInputT(RunSummarySceneContext& ctx) override;
    Transition updateT(RunSummarySceneContext& ctx, float dt) override;
    void drawT(RunSummarySceneContext& ctx) const override;

private:
    std::unique_ptr<RunSummarySceneContext> ownedCtx_;
    int  focused_   = 0;
    bool wantPick_  = false;
};
