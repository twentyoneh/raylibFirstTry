#include "SceneStack.h"

// вызываем InputHandler для текущей сцены
void SceneStack::handleInput() {
	if (auto* s = top()) s->handleInput(ctx_);
}

// обновляем сцены сверху вниз, пока не встретим блокирующую обновление сцену
void SceneStack::update(float deltaTime) {
	if (stack_.empty()) return;

	for (int i = static_cast<int>(stack_.size()) - 1; i >= 0; --i) {
		Transition t = stack_[i]->update(ctx_, deltaTime);
		if (t.type == TransitionType::Exit) {
			wantsExit_ = true;
			return;
		}
		else if (t.type != TransitionType::None) {
			apply(t);
			return;
		}
		if(stack_[i]->blocksUpdate()) break;
	}
}

void SceneStack::draw() const {
	if (wantsExit_) return;
	if (stack_.empty()) return;

	int first = 0;
	for (int i = static_cast<int>(stack_.size()) - 1; i >= 0; --i) {
		if (stack_[i]->blocksDraw()) { first = i; break; }
	}
	for (int i = first; i < static_cast<int>(stack_.size()); ++i) {
		stack_[i]->draw(const_cast<SceneContext&>(ctx_));
	}
}

// добавление в стек Сцены
void SceneStack::push(std::unique_ptr<Scene> s) {
	s->onEnter(ctx_);
	stack_.push_back(std::move(s));
}

void SceneStack::pop() {
	if (stack_.empty()) return;
	stack_.back()->onExit(ctx_);
	stack_.pop_back();
}

void SceneStack::swap(std::unique_ptr<Scene> s) {
	pop();
	push(std::move(s));
}

void SceneStack::apply(const Transition& t) {
	switch (t.type) {
	case TransitionType::None: break;
	case TransitionType::Pop:
		pop();
		break;
	case TransitionType::Push:
		if (t.factory) push(t.factory());
		break;
	case TransitionType::Swap:
		if (t.factory) swap(t.factory());
		break;
	}
}