#include "GameActor.h"



GameActor::GameActor(float x, float y) : x(x), y(y) {}


GameActor::~GameActor() {}

void GameActor::AMove() { x -= 1.0f; }
void GameActor::SMove() { y -= 1.0f; }
void GameActor::DMove() { x += 1.0f; }
void GameActor::WMove() { y += 1.0f; }
