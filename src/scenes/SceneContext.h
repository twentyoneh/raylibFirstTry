#pragma once
#include "../config/UiMenuTheme.h"
#include "../config/MenuInput.h"
#include "../gfx/TextureCache.h"

struct SceneContext {
	// окно / геометрия
	int screenW{ 1280 };
	int screenH{ 720 };
	TextureCache textures;

};