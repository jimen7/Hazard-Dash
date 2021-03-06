#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace std;
using namespace sf;

shared_ptr<sf::Texture> tex;
Sprite background;
shared_ptr<Entity> backEnt;
shared_ptr<Entity> txt1;
shared_ptr<Entity> txt2;
shared_ptr<Entity> txt3;
float dtMenu;
sf::Music musicMenu;




sf::Event Event;

void MenuScene::Load() {

	if (!musicMenu.openFromFile("res/Sounds/MainMenuMusic.wav")) {
		throw("Music File does not exist.");
	}
	musicMenu.setLoop(true);
	musicMenu.play();

	dtMenu = 0.0f;
	cout << "Menu Load \n";
	{
		tex = make_shared<sf::Texture>();
		if (!tex->loadFromFile("res/Sprites/Menu.png"))
		{
			throw ("Can't load menu image.");
		}
		backEnt = makeEntity();
		auto s = backEnt->addComponent<SpriteComponent>();
		s->setTexure(tex);
		// Text
		txt1 = makeEntity();
		txt2 = makeEntity();
		txt3 = makeEntity();
		auto t1 = txt1->addComponent<TextComponent>("Start Game");
		auto t2 = txt2->addComponent<TextComponent>("Options");
		auto t3 = txt3->addComponent<TextComponent>("Exit");
		t1->setPosition(sf::Vector2f(670.0f, 250.0f));
		t1->setSize(100);
		t2->setPosition(sf::Vector2f(750.0f, 450.0f));
		t2->setSize(100);
		t3->setPosition(sf::Vector2f(820.0f, 650.0f));
		t3->setSize(100);
	};

	setLoaded(true);
	Scene::Load();
}

void MenuScene::Update(const double& dt) {

	dtMenu += dt;
	// get the current mouse position in the window
	const sf::Vector2i pixelPos = sf::Mouse::getPosition(Engine::GetWindow());
	// convert it to world coordinate, because we scale in the render from 1080p to the target resolution
	const sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(pixelPos);

	if (worldPos.x > 676.0f && worldPos.x < 1272.0f && worldPos.y > 278.0f && worldPos.y < 359.0f) {
		txt1->GetCompatibleComponent<TextComponent>()[0]->setColour(Color::Red);
	}
	else if (worldPos.x > 747.0f && worldPos.x < 1174.0f && worldPos.y > 466.0f && worldPos.y < 562.0f) {
		txt2->GetCompatibleComponent<TextComponent>()[0]->setColour(Color::Red);
	}
	else if (worldPos.x > 826.0f && worldPos.x < 1056.0f && worldPos.y > 670.0f && worldPos.y < 759.0f) {
		txt3->GetCompatibleComponent<TextComponent>()[0]->setColour(Color::Red);
	}
	else {
		txt1->GetCompatibleComponent<TextComponent>()[0]->setColour(Color::White);
		txt2->GetCompatibleComponent<TextComponent>()[0]->setColour(Color::White);
		txt3->GetCompatibleComponent<TextComponent>()[0]->setColour(Color::White);
	}


	if (sf::Mouse::isButtonPressed(Engine::_Keysss["Click"].myMouseButton) && dtMenu > 0.2f) {
		if (worldPos.x > 676.0f && worldPos.x < 1272.0f && worldPos.y > 278.0f && worldPos.y < 359.0f) {
			Engine::ChangeScene(&testLevel);
			musicMenu.stop();
		}
		else if (worldPos.x > 747.0f && worldPos.x < 1174.0f && worldPos.y > 466.0f && worldPos.y < 562.0f) {
			Engine::ChangeScene(&optionsLevel);
		}
		else if (worldPos.x > 826.0f && worldPos.x < 1056.0f && worldPos.y > 670.0f && worldPos.y < 759.0f) {
			Engine::GetWindow().close();
		}
	}

	Scene::Update(dt);
}

