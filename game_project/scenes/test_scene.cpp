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
#include "../components/cmp_text.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <random>

# define M_PI           3.14159265358979323846  /* pi */

using namespace std;
using namespace sf;

//float tileSize;

bool xboxPlayer = false;


std::vector<sf::Text> trapDescriptions;
sf::Text textDes;
sf::Font font;
//PauseMenu Stuff
shared_ptr<sf::Texture> texMenu;
shared_ptr<Entity> EntMenu;

//shared_ptr<Entity> fireball;

bool key_pressed = false;

static shared_ptr<Entity> player;

float dtPauseMenu = 0.0f;



int heroes_num_test = 5;
std::vector<std::shared_ptr<Entity>> TestScene::heroes;	//Static Variable declare in header, we need it to be static cause heroes are changfed in the fireball trap component
// Text components for pause menu
std::shared_ptr<Entity> menuPause;

std::vector<int> indexOfheroesToRemove;

shared_ptr<sf::Texture> playerSpritesheet;

shared_ptr<sf::Texture> trapSpritesheet;

std::vector<sf::Vector2ul> doors;

std::vector<shared_ptr<Entity>> entityTrapsList;

int trapNum = 0;

shared_ptr<Entity> fireball;
bool testtest = true;

bool tile_sorted = false;


float tileSize;

sf::Music music;

// Game Loop Stuff
int roundNo = 1;
int heroes_num = 10;
bool gameplayPhase = false;
float timerGame = 2.0f;
float timerSpawn = 0.0f;
shared_ptr<Entity> RoundStatus;
int score = 0;
int highscore = 0;
int checkScore = 0;


void TestScene::Load() {
	//Read in the highscore text
	ifstream myfile("highscore.txt");
	if (!myfile.is_open()) {
		ofstream myfile2("highscore.txt");
		if (myfile2.is_open())
		{
			myfile2 << "0";
		}
		myfile2.close();
	}
	else {
		string line;
		while (getline(myfile, line))
		{
			highscore = std::stoi(line);
		}
		myfile.close();
	}


	if (!music.openFromFile("res/Sounds/Theme.wav")) {
		throw("Music File does not exist.");
	}
	music.setLoop(true);
	music.setVolume(Engine::getVolume());
	// float tileSize = (Engine::getWindowSize().y / 720.0f) * 20.0f;		//Original
	 //tileSize = (Engine::getWindowSize().y / 720.0f) * 20.0f;
	Scene::Load();
	tileSize = GAMEX / 64;
	cout << " Scene 1 Load" << endl;
	ls::loadLevelFile("res/main_level.txt", tileSize);
	ls::loadLevelFile("res/tiled/Dungeon_final.csv", tileSize);

	// PauseMenu stuff
	texMenu = make_shared<sf::Texture>();
	if (!texMenu->loadFromFile("res/Sprites/Menu_Pause.png"))
	{
		throw ("Can't load menu image.");
	}
	EntMenu = makeEntity2();
	RoundStatus = makeEntity();
	auto s = EntMenu->addComponent<SpriteComponent>();
	s->setTexure(texMenu);
	EntMenu->setPosition(sf::Vector2f(320, 180));

	auto t1 = EntMenu->addComponent<TextComponent>("Resume");
	auto t2 = EntMenu->addComponent<TextComponent>("Restart");
	auto t3 = EntMenu->addComponent<TextComponent>("Main Menu");
	auto t4 = EntMenu->addComponent<TextComponent>("Exit Game");
	auto t5 = RoundStatus->addComponent<TextComponent>("Round: " + to_string(roundNo) + ", Score: " + to_string(score) + ", HighScore: " + to_string(highscore));

	for (int i = 0; i < 4; i++) {
		EntMenu->GetCompatibleComponent<TextComponent>()[i]->setSize(100);
	}

	t1->setPosition(sf::Vector2f(760, 220));
	t2->setPosition(sf::Vector2f(730, 360));
	t3->setPosition(sf::Vector2f(670, 500));
	t4->setPosition(sf::Vector2f(670, 640));
	t5->setSize(50.0f);
	t5->setColour(Color::Black);
	t5->setPosition(sf::Vector2f(100, 30));


	// Sorting the door tiles so the player goes through them sequentially
	doors = ls::findTiles(ls::DOOR);
	ls::sortTiles(doors);

	// Sort the teleports
	auto teleports = ls::findTiles(ls::TELEPORT);
	ls::sortTiles(teleports);


	auto ho = GAMEY - (ls::getHeight() * tileSize);
	ls::setOffset(Vector2f(0, ho));


	music.play();

	//  TrapComponent::TestScene::heroes_list = &TestScene::heroes;		//Set the list of pointers to point to our hero list
	  // Add physics colliders to level tiles.
	{
		auto walls = ls::findTiles(ls::WALL);
		auto ground = ls::findTiles(ls::GROUND);
		auto traps = ls::findTiles(ls::TRAP);
		walls.insert(walls.end(), ground.begin(), ground.end());
		for (auto w : walls) {
			auto pos = ls::getTilePosition(w);
			pos += Vector2f(tileSize / DIVIDER, tileSize / DIVIDER); //offset to center
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
			if (!trapSpritesheet->loadFromFile("res/Sprites/Esquire3.png")) {	//This is just an empty sprite, but in order to change it a default one needs to be set
				cerr << "Failed to load spritesheet!" << std::endl;
			}
			auto s = e->addComponent<SpriteComponent>();
			s->setTexure(trapSpritesheet);
			s->setTextureRect(sf::IntRect(0, 128, 32, 32));
			s->rotateTexture(90.0f);

			//e->addComponent<PhysicsComponent>(false, Vector2f(tileSize, tileSize));
			e->addComponent<TrapComponent>(Vector2f(tileSize, tileSize));
			e->addComponent<TextComponent>("Empty");
			e->GetCompatibleComponent<TextComponent>()[0]->setPosition(e->getPosition() - Vector2f(tileSize / DIVIDER, tileSize));
			e->GetCompatibleComponent<TextComponent>()[0]->setSize(10);

			entityTrapsList.push_back(e);
		}
	}

	// Sorting the door tiles so the player goes through them sequentially
	{
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

	// Sorting the teleport tiles
	{
		srand(time(0));
		bool first = rand() & 1;
		cout << first << endl;
		for (int i = 0; i < teleports.size(); i++) {
			auto pos = ls::getTilePosition(teleports[i]);
			pos += Vector2f(tileSize / DIVIDER, tileSize / DIVIDER);
			auto e = makeEntity();
			e->setPosition(pos);
			if (i == 0 && first) {
				e->addComponent<DoorComponent>(Vector2f(tileSize, tileSize), ls::getTilePosition(teleports[2]));
			}
			else if (i == 0) {
				e->addComponent<DoorComponent>(Vector2f(tileSize, tileSize), ls::getTilePosition(doors[0]));
			}
			else if (i == 1 && !first) {
				e->addComponent<DoorComponent>(Vector2f(tileSize, tileSize), ls::getTilePosition(teleports[2]));
			}
			else if (i == 1) {
				e->addComponent<DoorComponent>(Vector2f(tileSize, tileSize), ls::getTilePosition(doors[0]));
			}
			e->addComponent<PhysicsComponent>(false, Vector2f(tileSize, tileSize));
		}
	}




	cout << " Scene 1 Load Done" << endl;

	setLoaded(true);
}

void TestScene::UnLoad() {
	cout << "Scene 1 Unload" << endl;
	TestScene::heroes.clear();
	entityTrapsList.clear();
	player.reset();
	ls::unload();
	Scene::UnLoad();
}

void TestScene::Update(const double& dt) {

	if (sf::Joystick::isButtonPressed(0, 7)) {
		xboxPlayer = true;
	}

	if (!tile_sorted) 	// Sorting the door tiles so the player goes through them sequentially
	{
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
		tile_sorted = true;
	}



	dtPauseMenu += dt;
	if (sf::Keyboard::isKeyPressed(Engine::_Keysss["Pause"].myKeyCode)) {
		if (!Engine::getPause() && dtPauseMenu > 0.2f) {
			dtPauseMenu = 0.0f;
			Engine::setPause(true);
		}
		else if (Engine::getPause() && dtPauseMenu > 0.2f) {
			dtPauseMenu = 0.0f;
			Engine::setPause(false);
		}
	}
	// DO Pause Menueue stuff here
	if (Engine::_gamePause) {
		// get the current mouse position in the window
		const sf::Vector2i pixelPos = sf::Mouse::getPosition(Engine::GetWindow());
		// convert it to world coordinate, because we scale in the render from 1080p to the target resolution
		const sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(pixelPos);

		if (worldPos.x > 771 && worldPos.x < 1117 && worldPos.y > 244 && worldPos.y < 331) {
			EntMenu->GetCompatibleComponent<TextComponent>()[0]->setColour(Color::Red);
		}
		else if (worldPos.x > 735 && worldPos.x < 1140 && worldPos.y > 390 && worldPos.y < 463) {
			EntMenu->GetCompatibleComponent<TextComponent>()[1]->setColour(Color::Red);
		}
		else if (worldPos.x > 676 && worldPos.x < 1189 && worldPos.y > 526 && worldPos.y < 607) {
			EntMenu->GetCompatibleComponent<TextComponent>()[2]->setColour(Color::Red);
		}
		else if (worldPos.x > 661 && worldPos.x < 1216 && worldPos.y > 666 && worldPos.y < 744) {
			EntMenu->GetCompatibleComponent<TextComponent>()[3]->setColour(Color::Red);
		}
		else {
			for (int i = 0; i < 4; i++) {
				EntMenu->GetCompatibleComponent<TextComponent>()[i]->setColour(Color::White);
			}
		}

		if (sf::Mouse::isButtonPressed(Engine::_Keysss["Click"].myMouseButton)) {
			if (worldPos.x > 771 && worldPos.x < 1117 && worldPos.y > 244 && worldPos.y < 331) {
				Engine::setPause(false);
			}
			else if (worldPos.x > 735 && worldPos.x < 1140 && worldPos.y > 390 && worldPos.y < 463) {
				score = 0;
				roundNo = 1;
				gameplayPhase = false;
				timerGame = 6.0f;
				Engine::ChangeScene(&menu);
				Engine::ChangeScene(&testLevel);
			}
			else if (worldPos.x > 676 && worldPos.x < 1189 && worldPos.y > 526 && worldPos.y < 607) {
				score = 0;
				roundNo = 1;
				gameplayPhase = false;
				timerGame = 6.0f;
				Engine::ChangeScene(&menu);
				music.stop();
			}
			else if (worldPos.x > 661 && worldPos.x < 1216 && worldPos.y > 666 && worldPos.y < 744) {
				Engine::GetWindow().close();
			}
		}
	}
	indexOfheroesToRemove.clear();
	Scene::Update(dt);
	// get the current mouse position in the window
	const sf::Vector2i pixelPos = sf::Mouse::getPosition(Engine::GetWindow());
	// convert it to world coordinate, because we scale in the render from 1080p to the target resolution
	const sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(pixelPos);


	if (!gameplayPhase && !Engine::_gamePause) {
		timerGame -= dt;
		if (timerGame < 0) {
			timerGame = 100.0f + 10.0f * roundNo;
			gameplayPhase = true;
			heroes_num = 10 * roundNo;
			checkScore = 0;
		}
	}
	else if (!Engine::_gamePause) {
		timerGame -= dt;
		timerSpawn += dt;

		int inc = 0;
		for (auto h : TestScene::heroes) {
			if (!h->isAlive())
				inc++;
			if (h->getPosition().y < 150) {
				if (score > highscore) {
					highscore = score;
					ofstream myfile2("highscore.txt");
					if (myfile2.is_open())
					{
						myfile2 << to_string(score);
					}
					myfile2.close();
				}
				RoundStatus->GetCompatibleComponent<TextComponent>()[0]->SetText("Game Over, Score: " + to_string(score) + ", HighScore: " + to_string(highscore));
				timerGame = 100000;
				//SaveScore();
			}

		}
		//Check if score has changed
		if (checkScore < inc) {
			checkScore++;
			score += 50;
			RoundStatus->GetCompatibleComponent<TextComponent>()[0]->SetText("Round: " + to_string(roundNo) + ", Score: " + to_string(score) + ", HighScore: " + to_string(highscore));
		}

		if (inc == heroes.size() && inc > 9)
			timerGame = -1;

		if (timerSpawn > 1.0f && heroes_num > 0) {
			timerSpawn = 0.0f;
			SpawnHero(heroes_num % 3);
			tile_sorted = false;
			heroes_num--;
		}
		if (timerGame < 0) {
			timerGame = 10.0f;
			gameplayPhase = false;
			for (auto h : TestScene::heroes) {
				h->setForDelete();
				h->setAlive(false);
			}
			TestScene::heroes.clear();
			roundNo++;

		}

	}




	for (int i = 0; i < TestScene::heroes.size(); i++) {
		if (TestScene::heroes[i] == NULL) {
			indexOfheroesToRemove.push_back(i);
		}
	}

	for (int i = 0; i < indexOfheroesToRemove.size(); i++) {
		TestScene::heroes.erase(TestScene::heroes.begin() + i);
	}


	for (auto t : entityTrapsList) {
		// Set to 18 as its half of tilesize (32)
		const auto dir = Vector2f(worldPos) - t->getPosition();//Gets mouse potition in relation to tile's
		const auto l = sf::length(dir);
		if (l < 25.0) {
			t->GetCompatibleComponent<TextComponent>()[0]->setSize(10);
			if (!(t->GetCompatibleComponent<TrapComponent>()[0]->isPlaced())) {
				if (sf::Mouse::isButtonPressed(Engine::_Keysss["Click"].myMouseButton)) {
					t->GetCompatibleComponent<TextComponent>()[0]->setPosition(t->getPosition() - Vector2f(tileSize / (DIVIDER), tileSize*3.0f));
					t->GetCompatibleComponent<TextComponent>()[0]->SetText("1: Mine\n2: Spikes\n3: Fireball Cannon Up\n4: Fireball Cannon Down\n5: Fireball Cannon Left\n6: Fireball Cannon Right\n");
				}

				if (Keyboard::isKeyPressed(Engine::_Keysss["Option 1"].myKeyCode)) {

					t->addComponent<PhysicsComponent>(false, Vector2f(tileSize, tileSize));
					t->addComponent<MineTrapComponent>(Vector2f(tileSize, tileSize));
					t->GetCompatibleComponent<TrapComponent>()[0]->setBoolPlaced();
					t->GetCompatibleComponent<TextComponent>()[0]->SetText("Mine");
					t->GetCompatibleComponent<TextComponent>()[0]->setPosition(t->getPosition() - Vector2f(tileSize / DIVIDER, tileSize));
				}

				if (Keyboard::isKeyPressed(Engine::_Keysss["Option 2"].myKeyCode)) {
					t->addComponent<PhysicsComponent>(false, Vector2f(tileSize, tileSize));
					t->addComponent<SpikeTrapComponent>(Vector2f(tileSize, tileSize));
					t->GetCompatibleComponent<TrapComponent>()[0]->setBoolPlaced();
					t->GetCompatibleComponent<TextComponent>()[0]->SetText("Spikes");
					t->GetCompatibleComponent<TextComponent>()[0]->setPosition(t->getPosition() - Vector2f(tileSize / DIVIDER, tileSize));
				}

				if (Keyboard::isKeyPressed(Engine::_Keysss["Option 3"].myKeyCode)) {
					t->addComponent<PhysicsComponent>(false, Vector2f(tileSize, tileSize));
					t->addComponent<FireballTrapComponent>(Vector2f(tileSize, tileSize), 0);
					t->GetCompatibleComponent<TrapComponent>()[0]->setBoolPlaced();
					t->GetCompatibleComponent<TextComponent>()[0]->SetText("Fireball Cannon Up");
					t->GetCompatibleComponent<TextComponent>()[0]->setPosition(t->getPosition() - Vector2f(tileSize / DIVIDER, tileSize));
				}

				if (Keyboard::isKeyPressed(Engine::_Keysss["Option 4"].myKeyCode)) {
					t->addComponent<PhysicsComponent>(false, Vector2f(tileSize, tileSize));
					t->addComponent<FireballTrapComponent>(Vector2f(tileSize, tileSize), 1);
					t->GetCompatibleComponent<TrapComponent>()[0]->setBoolPlaced();
					t->GetCompatibleComponent<TextComponent>()[0]->SetText("Fireball Cannon Down");
					t->GetCompatibleComponent<TextComponent>()[0]->setPosition(t->getPosition() - Vector2f(tileSize / DIVIDER, tileSize));
				}

				if (Keyboard::isKeyPressed(Engine::_Keysss["Option 5"].myKeyCode)) {

					t->addComponent<PhysicsComponent>(false, Vector2f(tileSize, tileSize));
					t->addComponent<FireballTrapComponent>(Vector2f(tileSize, tileSize), 2);
					t->GetCompatibleComponent<TrapComponent>()[0]->setBoolPlaced();
					t->GetCompatibleComponent<TextComponent>()[0]->SetText("Fireball Cannon Left");
					t->GetCompatibleComponent<TextComponent>()[0]->setPosition(t->getPosition() - Vector2f(tileSize / DIVIDER, tileSize));
				}

				if (Keyboard::isKeyPressed(Engine::_Keysss["Option 6"].myKeyCode)) {
					t->addComponent<PhysicsComponent>(false, Vector2f(tileSize, tileSize));
					t->addComponent<FireballTrapComponent>(Vector2f(tileSize, tileSize), 3);
					t->GetCompatibleComponent<TrapComponent>()[0]->setBoolPlaced();
					t->GetCompatibleComponent<TextComponent>()[0]->SetText("Fireball Cannon Right");
					t->GetCompatibleComponent<TextComponent>()[0]->setPosition(t->getPosition() - Vector2f(tileSize / DIVIDER, tileSize));
				}


			}
			else {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
					//t->del_components<TrapComponent>();
				}
			}

		}
		else {
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


void TestScene::SpawnHero(int i) {
	player = makeEntity();
	player->setPosition(ls::getTilePosition(ls::findTiles(ls::DOOR)[9]));
	std::shared_ptr<HealthComponent> condition;
	if (i == 0) {
		condition = player->addComponent<HealthComponent>(50.0f);
	}
	else if (i == 1) {
		condition = player->addComponent<HealthComponent>(100.0f);
	}
	else {
		condition = player->addComponent<HealthComponent>(150.0f);
	}
	playerSpritesheet = make_shared<sf::Texture>();

	auto s = player->addComponent<SpriteComponent>();


	playerSpritesheet->loadFromFile("res/Sprites/CharacterSheet.png");
	if (!playerSpritesheet->loadFromFile("res/Sprites/CharacterSheet.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}

	if (condition->getHealth() == 50.0f) {
		s->setTexure(playerSpritesheet);
		s->setTextureRect(sf::IntRect(128, 224, 32, 32));
	}
	else if (condition->getHealth() == 100.0f) {
		s->setTexure(playerSpritesheet);
		s->setTextureRect(sf::IntRect(128, 0, 32, 32));
	}
	else if (condition->getHealth() == 150.0f) {
		s->setTexure(playerSpritesheet);
		s->setTextureRect(sf::IntRect(128, 128, 32, 32));
	}


	player->addComponent<PlayerPhysicsComponent>(Vector2f(float(tileSize) / 2.0f, float(tileSize) * (3.0f / 4.0f)), doors);	//HOW IT WAS ORIGINALLY IMPLEMENTED


	if (xboxPlayer) {
		xboxPlayer = false;
	}
	else {
		player->addComponent<AIComponent>();
	}

	TestScene::heroes.push_back(player);				//When using TestScene::heroes list it thorws an error when pressing Escape. If we don't add any components to the player the rror doesn't happen. We believe this is a reference error. After debugging
											//we confirmed that the references are not deleted so that should not be causing an issue.
}