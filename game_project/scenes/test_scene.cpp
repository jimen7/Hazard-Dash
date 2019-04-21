#include "test_scene.h"
#include <SFML/Window/Mouse.hpp>
#include "../components/cmp_player_physics.h"
#include "../components/cmp_trap.h"
#include "../components/cmp_text.h"
#include "../components/cmp_door.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_health.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace sf;

//float tileSize;


std::vector<sf::Text> trapDescriptions;
sf::Text textDes;
sf::Font font;

static shared_ptr<Entity> player;

int heroes_num = 1;
std::vector<shared_ptr<Entity>> heroes;

std::vector<int> indexOfHeroesToRemove;

shared_ptr<sf::Texture> playerSpritesheet;


std::vector<shared_ptr<Entity>> entityTrapsList;

int trapNum = 0;


float tileSize;


void TestScene::Load() {
 // float tileSize = (Engine::getWindowSize().y / 720.0f) * 20.0f;		//Original
  tileSize = (Engine::getWindowSize().y / 720.0f) * 20.0f;
  cout << " Scene 1 Load" << endl;
  ls::loadLevelFile("res/main_level.txt", tileSize);
  
  // Sorting the door tiles so the player goes through them sequentially
  auto doors = ls::findTiles(ls::DOOR);
  ls::sortTiles(doors);



  auto ho = Engine::getWindowSize().y - (ls::getHeight() * tileSize);
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
		e->addComponent<TextComponent>("Empty");
		e->GetCompatibleComponent<TextComponent>()[0]->setPosition(e->getPosition() - Vector2f(15.f,30.f));
		e->GetCompatibleComponent<TextComponent>()[0]->setSize(10);
		//e->addComponent<SpikeTrapComponent>(Vector2f(tileSize, tileSize));
		//auto s = e->addComponent<ShapeComponent>();
		//s->setShape<RectangleShape>();
		//s->getShape().setFillColor(Color::Black);

		entityTrapsList.push_back(e);
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
		  ls::setColor(LevelSystem::TRAP, Color::Magenta);

	  }
  }

  
  //for (int i = 0; i < entityTrapsList.size(); i++) {
	 // // Load font-face from res dir
	 // font.loadFromFile("res/fonts/RobotoMono-Regular.ttf");
	 // // Set text element to use font
	 // textDes.setFont(font);
	 // // set the character size to 24 pixels
	 // textDes.setCharacterSize(0);
	 // //Set traps to be initially empty
	 // textDes.setString("Empty");
  //}



  cout << " Scene 1 Load Done" << endl;

  setLoaded(true);
}

void TestScene::UnLoad() {
  cout << "Scene 1 Unload" << endl;
  heroes.clear();
  entityTrapsList.clear();
  player.reset();
  ls::unload();
  Scene::UnLoad();
}

void TestScene::Update(const double& dt) {

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


		for (auto t : entityTrapsList) {
			

			const auto dir = Vector2f(sf::Mouse::getPosition()) - t->getPosition();//Gets mouse potition in relation to tile's
			const auto l = sf::length(dir);


			if (l < 40.0) {
				t->GetCompatibleComponent<TextComponent>()[0]->setSize(10);
				if (!(t->GetCompatibleComponent<TrapComponent>()[0]->isPlaced())) {


					if (Mouse::isButtonPressed(Mouse::Left)) {
						
						t->addComponent<MineTrapComponent>(Vector2f(tileSize, tileSize));
						t->GetCompatibleComponent<TrapComponent>()[0]->setBoolPlaced();
						t->GetCompatibleComponent<TextComponent>()[0]->SetText("Mine");
					}
				}
				else {
					if (Mouse::isButtonPressed(Mouse::Left)) {
						//t->removeComponent<TrapComponent>();
					}
				}
				
			}
			else{
				t->GetCompatibleComponent<TextComponent>()[0]->setSize(0);
			}

			
		}

}

void TestScene::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
