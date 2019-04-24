#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"
#include "scenes/options_menu.h"

using namespace std;




MenuScene menu;
TestScene testLevel;
OptionsScene optionsLevel;
RebindScene rebindLevel;


int main() {
  Engine::Start(1280, 720, "Platformer",&menu);
}