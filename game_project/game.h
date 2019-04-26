
#pragma once

#include "scenes/test_scene.h"

#include "scenes/scene_menu.h"
#include "scenes/options_menu.h"
#include "scenes/rebind_menu.h"



extern MenuScene menu;
extern TestScene testLevel;
extern OptionsScene optionsLevel;
extern RebindScene rebindLevel;
static bool toBeRebind=false;
static std::string actionDesc = "";
static int inputTypeVar = 0; //0 is joystic buttons, 1 is Joystic Axis, 2 is Keyboard Buttons, 3 is Mouse Buttons




