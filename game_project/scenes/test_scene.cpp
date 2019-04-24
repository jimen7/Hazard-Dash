#include "test_scene.h"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Audio.hpp>
#include "../components/cmp_player_physics.h"
#include "../components/cmp_trap.h"
#include "../components/cmp_text.h"
#include "../components/cmp_door.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_health.h"
#include "../components/cmp_AI.h"
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

shared_ptr<sf::Texture> trapSpritesheet;


std::vector<shared_ptr<Entity>> entityTrapsList;

int trapNum = 0;


float tileSize;

sf::Music music;

void TestScene::Load() {
	if (!music.openFromFile("res/Sounds/Theme.wav")) {
		throw("Music File does not exist.");
	}
	music.setLoop(true);
 // float tileSize = (Engine::getWindowSize().y / 720.0f) * 20.0f;		//Original
  //tileSize = (Engine::getWindowSize().y / 720.0f) * 20.0f;
  Scene::Load();
  tileSize = GAMEX / 64;
  cout << " Scene 1 Load" << endl;
  ls::loadLevelFile("res/main_level.txt", tileSize);
  ls::loadLevelFile("res/tiled/Dungeon_final.csv", tileSize);
  
  // Sorting the door tiles so the player goes through them sequentially
  auto doors = ls::findTiles(ls::DOOR);
  ls::sortTiles(doors);



  auto ho = GAMEY - (ls::getHeight() * tileSize);
  ls::setOffset(Vector2f(0, ho));

  // Create player

  for (int i = 0; i < heroes_num; i++) {
    player = makeEntity();
    player->setPosition(ls::getTilePosition(ls::findTiles(ls::DOOR)[9]));
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
	//s->setTextureRect(sf::IntRect(0, 0, 40, 40));
	s->setTextureRect(sf::IntRect(0, 0, 32, 32));


    player->addComponent<PlayerPhysicsComponent>(Vector2f(float(tileSize) / 2.0f ,float(tileSize) * (3.0f/4.0f)), doors);	//HOW IT WAS ORIGINALLY IMPLEMENTED
	player->addComponent<HealthComponent>(100.0f);
	//player->addComponent<AIComponent>();
	heroes.push_back(player);				//When using heroes list it thorws an error when pressing Escape. If we don't add any components to the player the rror doesn't happen. We believe this is a reference error. After debugging
											//we confirmed that the references are not deleted so that should not be causing an issue.
	music.play();
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
      pos += Vector2f(tileSize/ DIVIDER, tileSize / DIVIDER); //offset to center
      auto e = makeEntity();
      e->setPosition(pos);
      e->addComponent<PhysicsComponent>(false, Vector2f(tileSize, tileSize));
    }
	for (auto t : traps) {
		auto pos = ls::getTilePosition(t);
		pos += Vector2f(tileSize / DIVIDER, tileSize / DIVIDER); //offset to center
		auto e = makeEntity();
		e->setPosition(pos);

		trapSpritesheet = make_shared<sf::Texture>();

		trapSpritesheet->loadFromFile("res/Sprites/Esquire3.png");
		if (!trapSpritesheet->loadFromFile("res/Sprites/Esquire3.png")) {
			cerr << "Failed to load spritesheet!" << std::endl;
		}
		auto s = e->addComponent<SpriteComponent>();
		s->setTexure(trapSpritesheet);
		//s->setTextureRect(sf::IntRect(0, 0, 40, 40));
		s->setTextureRect(sf::IntRect(0, 128, 32, 32));
		//s->setTextureRect(sf::IntRect(0, 0, 32, 32));


		//e->addComponent<PhysicsComponent>(false, Vector2f(tileSize, tileSize));
		e->addComponent<TrapComponent>(Vector2f(tileSize, tileSize));
		e->addComponent<TextComponent>("Empty");
		e->GetCompatibleComponent<TextComponent>()[0]->setPosition(e->getPosition() - Vector2f(tileSize/DIVIDER,tileSize));
		e->GetCompatibleComponent<TextComponent>()[0]->setSize(10);
		//e->addComponent<SpikeTrapComponent>(Vector2f(tileSize, tileSize));
		//auto s = e->addComponent<ShapeComponent>();
		//s->setShape<RectangleShape>();
		//s->getShape().setFillColor(Color::Black);

		//sf::Mouse::setPosition(Vector2i(0.f,0.f));

		entityTrapsList.push_back(e);
	}
  }

  // Sorting the door tiles so the player goes through them sequentially
  {
	  std::vector<sf::Vector2ul> doors = ls::findTiles(ls::DOOR);
	  ls::sortTiles(doors);
	  for (int i = 0; i < doors.size(); i++) {
		  auto pos = ls::getTilePosition(doors[i]);
		  pos += Vector2f(tileSize / DIVIDER, tileSize / DIVIDER); //offset to center
		  auto e = makeEntity();
		  e->setPosition(pos);
		  if (i % 2 == 1 && i != doors.size() - 1)
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
//  if (ls::getTileAt(player->getPosition()) == ls::END) {
//    Engine::ChangeScene((Scene*)&level2);
//  }


	//player->GetCompatibleComponent<PhysicsComponent>()[0]->setRestitution(1.f);
	indexOfHeroesToRemove.clear();
	Scene::Update(dt);
	// get the current mouse position in the window
	const sf::Vector2i pixelPos = sf::Mouse::getPosition(Engine::GetWindow());
	// convert it to world coordinate, because we scale in the render from 1080p to the target resolution
	const sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(pixelPos);
		
		for (int i = 0; i < heroes.size();i++) {
			if (heroes[i]==NULL) {
				indexOfHeroesToRemove.push_back(i);
			}
		}

		for (int i = 0; i < indexOfHeroesToRemove.size(); i++) {
			heroes.erase(heroes.begin()+i);
		}


		for (auto t : entityTrapsList) {
			// Set to 18 as its half of tilesize (32)
			const auto dir = Vector2f(worldPos) - t->getPosition();//Gets mouse potition in relation to tile's
			const auto l = sf::length(dir);
			if (l < 25.0) {
				t->GetCompatibleComponent<TextComponent>()[0]->setSize(10);
				if (!(t->GetCompatibleComponent<TrapComponent>()[0]->isPlaced())) {
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
						t->GetCompatibleComponent<TextComponent>()[0]->setPosition(t->getPosition() - Vector2f(tileSize / DIVIDER, tileSize));
						t->GetCompatibleComponent<TextComponent>()[0]->SetText("Num1: Mine\nNum2: Spikes");
					}

					if (Keyboard::isKeyPressed(Keyboard::Num1)) {
						
						t->addComponent<PhysicsComponent>(false, Vector2f(tileSize, tileSize));
						t->addComponent<MineTrapComponent>(Vector2f(tileSize, tileSize));
						t->GetCompatibleComponent<TrapComponent>()[0]->setBoolPlaced();
						t->GetCompatibleComponent<TextComponent>()[0]->SetText("Mine");
						t->GetCompatibleComponent<TextComponent>()[0]->setPosition(t->getPosition() - Vector2f(tileSize / DIVIDER, tileSize));
					}

					if (Keyboard::isKeyPressed(Keyboard::Num2)) {

						t->addComponent<PhysicsComponent>(false, Vector2f(tileSize, tileSize));
						t->addComponent<SpikeTrapComponent>(Vector2f(tileSize, tileSize));
						t->GetCompatibleComponent<TrapComponent>()[0]->setBoolPlaced();
						t->GetCompatibleComponent<TextComponent>()[0]->SetText("Spikes");
						t->GetCompatibleComponent<TextComponent>()[0]->setPosition(t->getPosition() - Vector2f(tileSize / DIVIDER, tileSize));
					}
				}
				else {
					if (Mouse::isButtonPressed(Mouse::Right)) {
						//t->removeComponent<TrapComponent>();
					}
				}
				
			}
			else{
				t->GetCompatibleComponent<TextComponent>()[0]->setSize(0);
				if (!(t->GetCompatibleComponent<TrapComponent>()[0]->isPlaced())) {
					t->GetCompatibleComponent<TextComponent>()[0]->SetText("Empty");
				}
				
			}

			
		}



}

void TestScene::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
