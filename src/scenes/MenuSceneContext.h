#pragma once
#include "SceneContext.h"

struct MenuScheneContext : SceneContext
{
	UiMenuTheme ui{};
	MenuInput   input{};
};