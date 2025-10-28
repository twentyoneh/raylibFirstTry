#include "GameActor.h"



GameActor::GameActor(float x, float y) : x(static_cast<int>(x)), y(static_cast<int>(y)) {}


GameActor::~GameActor() {}

void GameActor::AMove() { x -= 1; }
void GameActor::SMove() { y -= 1; }
void GameActor::DMove() { x += 1; }
void GameActor::WMove() { y += 1; }
