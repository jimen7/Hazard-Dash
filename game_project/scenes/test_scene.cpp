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

	if (Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
		//  ------------------------------------------------- 
		Engine::GetWindow().setSize({ 1000, 1000 });
		//  ------------------------------------------------- 
	}
	else if (Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
		//  ------------------------------------------------- 
		Engine::GetWindow().setSize({ 1280, 720 });
		//  ------------------------------------------------- 
	}
	else if (Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
		//  ------------------------------------------------- 
		// Change viewport to match window size
		// Doesn't scale anything! But now we get clipping!
		Engine::GetWindow().setSize({ 1280, 720 });
		sf::FloatRect visibleArea(0.f, 0.f, 1280, 720);
		Engine::GetWindow().setView(sf::View(visibleArea));
		//  ------------------------------------------------- 
	}
	else if (Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
		//  ------------------------------------------------- 
		// Change viewport to match window size
		Engine::GetWindow().setSize({ 1280, 720 });
		sf::FloatRect visibleArea(0.f, 0.f, 1000, 1000);
		auto v = sf::View(visibleArea);

		// constrain 1000x1000 to 1280x720 & Maintain Aspect. so 720x720;
		const float widthScale(720.0 / 1280.0);
		v.setViewport(sf::FloatRect(0, 0, widthScale, 1.0f));
		Engine::GetWindow().setView(v);
		//  ------------------------------------------------- 
	}
	else if (Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
		//  ------------------------------------------------- 
		// All together now in a reusable solution.
		const sf::Vector2u screensize(1280, 720);
		const sf::Vector2u gamesize(GAMEX, GAMEY);
		//set View as normal
		Engine::GetWindow().setSize(screensize);
		sf::FloatRect visibleArea(0.f, 0.f, gamesize.x, gamesize.y);
		auto v = sf::View(visibleArea);
		// figure out how to scale and maintain aspect;
		auto viewport = CalculateViewport(screensize, gamesize);
		//Optionally Center it
		bool centered = true;
		if (centered) {
			viewport.left = (1.0 - viewport.width) * 0.5;
			viewport.top = (1.0 - viewport.height) * 0.5;
		}
		//set!
		v.setViewport(viewport);
		Engine::GetWindow().setView(v);
		//  ------------------------------------------------- 
	}
	else if (Keyboard::isKeyPressed(sf::Keyboard::Num6)) {
		//  ------------------------------------------------- 
		// Have to Hold 6 Key for this to work magic
		static float t = 0;
		t += dt;
		const sf::Vector2u screensize((sin(t) + 1.3) *0.5 * 1000,
			(cos(t) + 1.3)* 0.5 * 800);

		const sf::Vector2u gamesize(GAMEX, GAMEY);
		Engine::GetWindow().setSize(screensize);
		sf::FloatRect visibleArea(0.f, 0.f, gamesize.x, gamesize.y);
		auto v = sf::View(visibleArea);
		auto viewport = CalculateViewport(screensize, gamesize);
		viewport.left = (1.0 - viewport.width) * 0.5;
		viewport.top = (1.0 - viewport.height) * 0.5;
		v.setViewport(viewport);
		Engine::GetWindow().setView(v);
	}


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
