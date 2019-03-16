#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"

using namespace std;

MenuScene menu;
TestScene testLevel;


int main() {
  Engine::Start(1980, 1080, "Platformer",&menu);
}