#pragma once
#include "Scene.h"

template <class Ctx>
class TypedScene : public Scene {
public:
	explicit TypedScene(Ctx& ctx) : ctx_(ctx) {}

	// адаптация базовых вызовов
	void onEnter(SceneContext&) override { onEnterT(ctx_); }
	void onExit(SceneContext&) override { onExitT(ctx_); }
	void handleInput(SceneContext&) override { handleInputT(ctx_); }
	void draw(SceneContext&) const override { drawT(ctx_); }
	Transition update(SceneContext&, float dt) override { return updateT(ctx_, dt); }

	// сцена реализует эти методы
	virtual void onEnterT(Ctx&) {}
	virtual void onExitT(Ctx&) {}
	virtual void handleInputT(Ctx&) = 0;
	virtual void drawT(Ctx&) const = 0;
	virtual Transition updateT(Ctx&, float) { return Transition::None(); };

private:
	Ctx& ctx_;
};