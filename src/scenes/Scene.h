#pragma once
#include <functional>
#include <memory>
#include "SceneContext.h"

enum class TransitionType
{
	None,
	Push,
	Pop,
	Swap,
	Exit
};

struct Transition
{
	TransitionType type{ TransitionType::None };

	std::function<std::unique_ptr<class Scene>()> factory;

	static Transition None() { return {}; }

	static Transition Exit() { return { TransitionType::Exit, {} }; }

	template <class F> 
	static Transition Push(F&& factory)
	{
		return { TransitionType::Push, std::forward<F>(factory) };
	}

	static Transition Pop()
	{
		return { TransitionType::Pop, {} };
	}

	template <class F>
	static Transition Swap(F&& factory)
	{
		return { TransitionType::Swap, std::forward<F>(factory) };
	}
};

class Scene
{
public:
	virtual ~Scene() = default;
	
	// Иницилизация
	virtual void onEnter(SceneContext&) {}
	virtual void onExit(SceneContext&) {}

	virtual void handleInput(SceneContext&) = 0;

	virtual Transition update(SceneContext&, float) = 0;
	virtual void draw(SceneContext&) const = 0;

	virtual bool blocksUpdate() const { return true; }
	virtual bool blocksDraw()   const { return true; }
};