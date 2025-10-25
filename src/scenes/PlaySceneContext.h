#pragma once
#include "SceneContext.h"
#include "../config/PlayInput.h"

struct PlaySceneContext : SceneContext
{
	PlayInput input{};
};