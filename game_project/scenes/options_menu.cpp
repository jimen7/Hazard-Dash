#include "options_menu.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

shared_ptr<sf::Texture> tex2;
Sprite background2;
shared_ptr<Entity> backEnt2;
shared_ptr<Entity> txt;


void OptionsScene::Load() {
	cout << "Menu Load \n";
	{
		tex2 = make_shared<sf::Texture>();
		if (!tex2->loadFromFile("res/Sprites/Menu_Options.png"))
		{
			throw ("Can't load menu image.");
		}
		backEnt2 = makeEntity();
		auto s = backEnt2->addComponent<SpriteComponent>();
		s->setTexure(tex2);
		// Text
		txt = makeEntity();
		auto t1 = txt->addComponent<TextComponent>("Volume");
		auto t2 = txt->addComponent<TextComponent>("Resolution");
		auto t3 = txt->addComponent<TextComponent>("Back");
		auto t4 = txt->addComponent<TextComponent>("Set Keybindings");
		auto t5 = txt->addComponent<TextComponent>("1280x720");
		auto t6 = txt->addComponent<TextComponent>("1920x1080");
		auto t7 = txt->addComponent<TextComponent>("2560x1440");
		auto t8 = txt->addComponent<TextComponent>("3840x2160");
		auto t9 = txt->addComponent<TextComponent>("Windowed");
		auto t10 = txt->addComponent<TextComponent>("Yes");
		auto t11 = txt->addComponent<TextComponent>("No");

		t1->setPosition(sf::Vector2f(520.0f, 300.0f));
		t1->setSize(40);
		t2->setPosition(sf::Vector2f(520, 450.0f));
		t2->setSize(40);
		t3->setPosition(sf::Vector2f(830.0f, 720.0f));
		t3->setSize(40);
		t4->setPosition(sf::Vector2f(830.0f, 560.0f));
		t4->setSize(40);
		t5->setPosition(sf::Vector2f(830.0f, 410.0f));
		t5->setSize(40);
		t6->setPosition(sf::Vector2f(1100.0f, 410.0f));
		t6->setSize(40);
		t7->setPosition(sf::Vector2f(830.0f, 470.0f));
		t7->setSize(40);
		t8->setPosition(sf::Vector2f(1100.0f, 470.0f));
		t8->setSize(40);
		t9->setPosition(sf::Vector2f(520.0f, 645.0f));
		t9->setSize(40);
		t10->setPosition(sf::Vector2f(830.0f, 645.0f));
		t10->setSize(40);
		t11->setPosition(sf::Vector2f(1000.0f, 645.0f));
		t11->setSize(40);
	};

	setLoaded(true);
	Scene::Load();
}

void OptionsScene::Update(const double& dt) {

	// get the current mouse position in the window
	const sf::Vector2i pixelPos = sf::Mouse::getPosition(Engine::GetWindow());
	// convert it to world coordinate, because we scale in the render from 1080p to the target resolution
	const sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(pixelPos);

	if (worldPos.x > 676.0f && worldPos.x < 1272.0f && worldPos.y > 278.0f && worldPos.y < 359.0f) {
		txt->GetCompatibleComponent<TextComponent>()[0]->setColour(Color::Red);
	}
	else if (worldPos.x > 747.0f && worldPos.x < 1174.0f && worldPos.y > 466.0f && worldPos.y < 562.0f) {
		txt->GetCompatibleComponent<TextComponent>()[0]->setColour(Color::Red);
	}
	else if (worldPos.x > 826.0f && worldPos.x < 1056.0f && worldPos.y > 670.0f && worldPos.y < 759.0f) {
		txt->GetCompatibleComponent<TextComponent>()[0]->setColour(Color::Red);
	}
	else {
		txt->GetCompatibleComponent<TextComponent>()[0]->setColour(Color::White);
		txt->GetCompatibleComponent<TextComponent>()[0]->setColour(Color::White);
		txt->GetCompatibleComponent<TextComponent>()[0]->setColour(Color::White);
	}


	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (worldPos.x > 676.0f && worldPos.x < 1272.0f && worldPos.y > 278.0f && worldPos.y < 359.0f) {
			Engine::ChangeScene(&testLevel);
		}
		else if (worldPos.x > 747.0f && worldPos.x < 1174.0f && worldPos.y > 466.0f && worldPos.y < 562.0f) {
			//Engine::ChangeScene(&testLevel);
		}
		else if (worldPos.x > 826.0f && worldPos.x < 1056.0f && worldPos.y > 670.0f && worldPos.y < 759.0f) {
			Engine::ChangeScene(&menu);
		}
	}

	Scene::Update(dt);
}

