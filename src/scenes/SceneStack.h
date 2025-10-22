#pragma once
#include <vector>
#include <memory>
#include "Scene.h"

class SceneStack
{
public:
	explicit SceneStack(SceneContext& ctx) : ctx_(ctx) {}
	
	void handleInput();
	void update(float deltaTime);
	void draw() const;

	void push(std::unique_ptr<Scene> s);
	void pop();
	void swap(std::unique_ptr<Scene> s);

	Scene* top() { return stack_.empty() ? nullptr : stack_.back().get(); }

private:
	void apply(const Transition& t);

	SceneContext& ctx_;
	std::vector<std::unique_ptr<Scene>> stack_;
};