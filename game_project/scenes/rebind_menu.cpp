#include "rebind_menu.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

shared_ptr<sf::Texture> tex3;
Sprite background3;
shared_ptr<Entity> backEnt3;
shared_ptr<Entity> txt4;
float dtRebind;


void RebindScene::Load() {
	dtRebind = 0.0f;
	{
		tex3 = make_shared<sf::Texture>();
		if (!tex3->loadFromFile("res/Sprites/Menu.png"))
		{
			throw ("Can't load menu image.");
		}
		backEnt3 = makeEntity();
		auto s = backEnt3->addComponent<SpriteComponent>();
		s->setTexure(tex3);
		// Text
		txt4 = makeEntity();
		// TODO Replace insert with actual values
		auto t1 = txt4->addComponent<TextComponent>("Jump: INSERT");
		auto t2 = txt4->addComponent<TextComponent>("Left: INSERT");
		auto t3 = txt4->addComponent<TextComponent>("Right: INSERT");
		auto t4 = txt4->addComponent<TextComponent>("Pause: INSERT");
		auto t5 = txt4->addComponent<TextComponent>("Mouse Click: INSERT");
		auto t6 = txt4->addComponent<TextComponent>("Back");
		auto t7 = txt4->addComponent<TextComponent>("");
		
		t1->setPosition(sf::Vector2f(520.0f, 300.0f));
		t1->setSize(40);
		t2->setPosition(sf::Vector2f(520, 400.0f));
		t2->setSize(40);
		t3->setPosition(sf::Vector2f(520, 500));
		t3->setSize(40);
		t4->setPosition(sf::Vector2f(520, 600));
		t4->setSize(40);
		t5->setPosition(sf::Vector2f(520, 700));
		t5->setSize(40);
		t6->setPosition(sf::Vector2f(1200, 700));
		t6->setSize(40);
		t7->setPosition(sf::Vector2f(900, 450));
		t7->setSize(40);
	};
	setLoaded(true);
	Scene::Load();
}

void RebindScene::Update(const double& dt) {
	dtRebind += dt;
	// get the current mouse position in the window
	const sf::Vector2i pixelPos = sf::Mouse::getPosition(Engine::GetWindow());
	// convert it to world coordinate, because we scale in the render from 1080p to the target resolution
	const sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(pixelPos);

	if (worldPos.x > 552 && worldPos.x < 733 && worldPos.y > 303 && worldPos.y < 352) {
		txt4->GetCompatibleComponent<TextComponent>()[0]->setColour(Color::Red);
	}
	else if (worldPos.x > 523 && worldPos.x < 741 && worldPos.y > 406 && worldPos.y < 450) {
		txt4->GetCompatibleComponent<TextComponent>()[1]->setColour(Color::Red);
	}
	else if (worldPos.x > 525 && worldPos.x < 753 && worldPos.y > 504 && worldPos.y < 547) {
		txt4->GetCompatibleComponent<TextComponent>()[2]->setColour(Color::Red);
	}
	else if (worldPos.x > 523 && worldPos.x < 738 && worldPos.y > 600 && worldPos.y < 658) {
		txt4->GetCompatibleComponent<TextComponent>()[3]->setColour(Color::Red);
	}
	else if (worldPos.x > 523 && worldPos.x < 885 && worldPos.y > 700 && worldPos.y < 757) {
		txt4->GetCompatibleComponent<TextComponent>()[4]->setColour(Color::Red);
	}
	else if (worldPos.x > 1203 && worldPos.x < 1296 && worldPos.y > 706 && worldPos.y < 744) {
		txt4->GetCompatibleComponent<TextComponent>()[5]->setColour(Color::Red);
	}
	else {
		for (int i = 0; i < 6; i++) {
			txt4->GetCompatibleComponent<TextComponent>()[i]->setColour(Color::White);
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && dtRebind > 0.2f) {
		dtRebind = 0.0f;
		if (worldPos.x > 1203 && worldPos.x < 1296 && worldPos.y > 706 && worldPos.y < 744) {
			Engine::ChangeScene(&optionsLevel);
		}
		// TODO DIMITRIOS YOU DO YOUR THING BELOW
		else if (worldPos.x > 552 && worldPos.x < 733 && worldPos.y > 303 && worldPos.y < 352) {
			txt4->GetCompatibleComponent<TextComponent>()[6]->SetText("Press Button to\nconfirm choice");
		}
		else if (worldPos.x > 523 && worldPos.x < 741 && worldPos.y > 406 && worldPos.y < 450) {
			txt4->GetCompatibleComponent<TextComponent>()[6]->SetText("Press Button to\nconfirm choice");
		}
		else if (worldPos.x > 525 && worldPos.x < 753 && worldPos.y > 504 && worldPos.y < 547) {
			txt4->GetCompatibleComponent<TextComponent>()[6]->SetText("Press Button to\nconfirm choice");
		}
		else if (worldPos.x > 523 && worldPos.x < 738 && worldPos.y > 600 && worldPos.y < 658) {
			txt4->GetCompatibleComponent<TextComponent>()[6]->SetText("Press Button to\nconfirm choice");
		}
		else if (worldPos.x > 523 && worldPos.x < 885 && worldPos.y > 700 && worldPos.y < 757) {
			txt4->GetCompatibleComponent<TextComponent>()[6]->SetText("Press Button to\nconfirm choice");
		}

	}

	Scene::Update(dt);
}
