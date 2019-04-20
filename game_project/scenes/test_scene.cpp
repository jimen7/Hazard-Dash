#include "test_scene.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_trap.h"
#include "../components/cmp_door.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_health.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;

int heroes_num = 1;
std::vector<shared_ptr<Entity>> heroes;

std::vector<int> indexOfHeroesToRemove;

shared_ptr<sf::Texture> playerSpritesheet;





void TestScene::Load() {
  Scene::Load();
  float tileSize = GAMEX / 64;
  cout << " Scene 1 Load" << endl;
  ls::loadLevelFile("res/main_level.txt", tileSize);
  
  // Sorting the door tiles so the player goes through them sequentially
  auto doors = ls::findTiles(ls::DOOR);
  ls::sortTiles(doors);



  auto ho = GAMEY - (ls::getHeight() * tileSize);
  ls::setOffset(Vector2f(0, ho));

  // Create player

  for (int i = 0; i < heroes_num; i++) {
    player = makeEntity();
    player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
    //auto s = player->addComponent<ShapeComponent>();
    //s->setShape<sf::RectangleShape>(Vector2f(tileSize / 2.0f, tileSize * (3.0f / 4.0f)));
    //s->getShape().setFillColor(Color::Magenta);
    //s->getShape().setOrigin(tileSize / 4.0f, tileSize * (3.0f / 8.0f));

	playerSpritesheet = make_shared<sf::Texture>();

	playerSpritesheet->loadFromFile("res/Sprites/Esquire2.png");
	if (!playerSpritesheet->loadFromFile("res/Sprites/Esquire2.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}

	auto s = player->addComponent<SpriteComponent>();
	s->setTexure(playerSpritesheet);
	s->setTextureRect(sf::IntRect(0, 0, 32, 32));


    player->addComponent<PlayerPhysicsComponent>(Vector2f(tileSize / 2.0f, tileSize * (3.0f/4.0f)), doors);
	player->addComponent<HealthComponent>(100.0f);
	heroes.push_back(player);				//When using heroes list it thorws an error when pressing Escape. If we don't add any components to the player the rror doesn't happen. We believe this is a reference error. After debugging
											//we confirmed that the references are not deleted so that should not be causing an issue.
  }



//  TrapComponent::heroes_list = &heroes;		//Set the list of pointers to point to our hero list
  // Add physics colliders to level tiles.
  {
    auto walls = ls::findTiles(ls::WALL);
	auto ground = ls::findTiles(ls::GROUND);
	auto traps = ls::findTiles(ls::TRAP);
	walls.insert(walls.end(), ground.begin(), ground.end());
    for (auto w : walls) {
      auto pos = ls::getTilePosition(w);
      pos += Vector2f(tileSize/2 , tileSize /2); //offset to center
      auto e = makeEntity();
      e->setPosition(pos);
      e->addComponent<PhysicsComponent>(false, Vector2f(tileSize, tileSize));
    }
	for (auto t : traps) {
		auto pos = ls::getTilePosition(t);
		pos += Vector2f(tileSize / 2, tileSize / 2); //offset to center
		auto e = makeEntity();
		e->setPosition(pos);
		e->addComponent<PhysicsComponent>(false, Vector2f(tileSize, tileSize));
		e->addComponent<TrapComponent>(Vector2f(tileSize, tileSize));
	}
  }

  // Sorting the door tiles so the player goes through them sequentially
  {
	  std::vector<sf::Vector2ul> doors = ls::findTiles(ls::DOOR);
	  ls::sortTiles(doors);
	  for (int i = 0; i < doors.size(); i++) {
		  auto pos = ls::getTilePosition(doors[i]);
		  pos += Vector2f(tileSize / 2, tileSize / 2); //offset to center
		  auto e = makeEntity();
		  e->setPosition(pos);
		  if (i % 2 == 0 && i != doors.size() - 1)
 			  e->addComponent<DoorComponent>(Vector2f(tileSize, tileSize), ls::getTilePosition(doors[i + 1]));
		  else
			  e->addComponent<DoorComponent>(Vector2f(tileSize, tileSize));
		  e->addComponent<PhysicsComponent>(false, Vector2f(tileSize, tileSize));

	  }
  }


  cout << " Scene 1 Load Done" << endl;

  setLoaded(true);
}

void TestScene::UnLoad() {
  cout << "Scene 1 Unload" << endl;
  heroes.clear();
  player.reset();
  ls::unload();
  Scene::UnLoad();
}

void TestScene::Update(const double& dt) {
//  if (ls::getTileAt(player->getPosition()) == ls::END) {
//    Engine::ChangeScene((Scene*)&level2);
//  }

	indexOfHeroesToRemove.clear();
		Scene::Update(dt);

		
		for (int i = 0; i < heroes.size();i++) {
			if (heroes[i]==NULL) {
				indexOfHeroesToRemove.push_back(i);
			}
		}

		for (int i = 0; i < indexOfHeroesToRemove.size(); i++) {
			heroes.erase(heroes.begin()+i);
		}
}

void TestScene::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
