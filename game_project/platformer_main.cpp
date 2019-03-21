#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"

using namespace std;

MenuScene menu;
TestScene testLevel;


int main() {
  Engine::Start(2560, 1440, "Platformer",&menu);
}