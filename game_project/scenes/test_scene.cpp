#include "test_scene.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;

shared_ptr<sf::Texture> playerSpritesheet;


void TestScene::Load() {
  float tileSize = (Engine::getWindowSize().y / 720.0f) * 40.0f;
  cout << " Scene 1 Load" << endl;
  ls::loadLevelFile("res/test_level.txt", tileSize);

  auto ho = Engine::getWindowSize().y - (ls::getHeight() * tileSize);
  ls::setOffset(Vector2f(0, ho));

  // Create player
  {
    player = makeEntity();
    player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
    //auto s = player->addComponent<ShapeComponent>();
    //s->setShape<sf::RectangleShape>(Vector2f(tileSize / 2.0f, tileSize * (3.0f / 4.0f)));
    //s->getShape().setFillColor(Color::Magenta);
    //s->getShape().setOrigin(tileSize / 4.0f, tileSize * (3.0f / 8.0f));

	playerSpritesheet = make_shared<sf::Texture>();

	playerSpritesheet->loadFromFile("res/Sprites/Esquire2.png");
	if (!playerSpritesheet->loadFromFile("res/img/invaders_sheet.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}

	auto s = player->addComponent<SpriteComponent>();
	s->setTexure(playerSpritesheet);
	s->setTextureRect(sf::IntRect(0, 0, 32, 32));


    player->addComponent<PlayerPhysicsComponent>(Vector2f(tileSize / 2.0f, tileSize * (3.0f/4.0f)));
  }

  // Add physics colliders to level tiles.
  {
    auto walls = ls::findTiles(ls::WALL);
    for (auto w : walls) {
      auto pos = ls::getTilePosition(w);
      pos += Vector2f(tileSize / 2.0f, tileSize / 2.0f); //offset to center
      auto e = makeEntity();
      e->setPosition(pos);
      e->addComponent<PhysicsComponent>(false, Vector2f(tileSize, tileSize));
    }
  }



  cout << " Scene 1 Load Done" << endl;

  setLoaded(true);
}

void TestScene::UnLoad() {
  cout << "Scene 1 Unload" << endl;
  player.reset();
  ls::unload();
  Scene::UnLoad();
}

void TestScene::Update(const double& dt) {

  if (ls::getTileAt(player->getPosition()) == ls::END) {
//    Engine::ChangeScene((Scene*)&level2);
  }
  Scene::Update(dt);
}

void TestScene::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
