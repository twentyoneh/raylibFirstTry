#pragma once
#include "SceneContext.h"
#include "../config/PlayInput.h"
#include "../units/BulletFactory.h"

struct PlaySceneContext : SceneContext
{
	PlayInput input{};
	BulletFactory bullets{};
};