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
float dtOptions;


void OptionsScene::Load() {
	dtOptions = 0.0f;
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
	dtOptions += dt;
	// get the current mouse position in the window
	const sf::Vector2i pixelPos = sf::Mouse::getPosition(Engine::GetWindow());
	// convert it to world coordinate, because we scale in the render from 1080p to the target resolution
	const sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(pixelPos);

	if (worldPos.x > 847.0f && worldPos.x < 1377.0f && worldPos.y > 304.0f && worldPos.y < 349.0f) {
		txt->GetCompatibleComponent<TextComponent>()[0]->setColour(Color::Red);
	}
	else if (worldPos.x > 834.0f && worldPos.x < 1021.0f && worldPos.y > 417.0f && worldPos.y < 453.0f) {
		txt->GetCompatibleComponent<TextComponent>()[4]->setColour(Color::Red);
	}
	else if (worldPos.x > 1104.0f && worldPos.x < 1320.0f && worldPos.y > 417.0f && worldPos.y < 448.0f) {
		txt->GetCompatibleComponent<TextComponent>()[5]->setColour(Color::Red);
	}
	else if (worldPos.x > 837.0f && worldPos.x < 1047.0f && worldPos.y > 478.0f && worldPos.y < 510.0f) {
		txt->GetCompatibleComponent<TextComponent>()[6]->setColour(Color::Red);
	}
	else if (worldPos.x > 1101.0f && worldPos.x < 1314.0f && worldPos.y > 478.0f && worldPos.y < 510.0f) {
		txt->GetCompatibleComponent<TextComponent>()[7]->setColour(Color::Red);
	}
	else if (worldPos.x > 834.0f && worldPos.x < 1192.0f && worldPos.y > 570.0f && worldPos.y < 612.0f) {
		txt->GetCompatibleComponent<TextComponent>()[3]->setColour(Color::Red);
	}
	else if (worldPos.x > 832.0f && worldPos.x < 897.0f && worldPos.y > 655.0f && worldPos.y < 688.0f) {
		txt->GetCompatibleComponent<TextComponent>()[9]->setColour(Color::Red);
	}
	else if (worldPos.x > 1002.0f && worldPos.x < 1044.0f && worldPos.y > 652.0f && worldPos.y < 693.0f) {
		txt->GetCompatibleComponent<TextComponent>()[10]->setColour(Color::Red);
	}
	else if (worldPos.x > 832.0f && worldPos.x < 927.0f && worldPos.y > 727.0f && worldPos.y < 763.0f) {
		txt->GetCompatibleComponent<TextComponent>()[2]->setColour(Color::Red);
	}
	else {
		for (int i = 0; i < 11; i++) {
			txt->GetCompatibleComponent<TextComponent>()[i]->setColour(Color::White);
		}
	}

	if (sf::Mouse::isButtonPressed(Engine::_Keysss["Click"].myMouseButton) && dtOptions > 0.2f) {
		dtOptions = 0.0f;
		if (worldPos.x > 834.0f && worldPos.x < 1021.0f && worldPos.y > 417.0f && worldPos.y < 453.0f) {
			const sf::Vector2u gamesize(GAMEX, GAMEY);
			const sf::Vector2u screensize = sf::Vector2u(1280, 720);
			Engine::GetWindow().setSize(screensize);
			//set View as normal
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
		}
		else if (worldPos.x > 1104.0f && worldPos.x < 1320.0f && worldPos.y > 417.0f && worldPos.y < 448.0f) {
			const sf::Vector2u gamesize(GAMEX, GAMEY);
			const sf::Vector2u screensize = sf::Vector2u(1920, 1080);
			Engine::GetWindow().setSize(screensize);
			//set View as normal
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
		}
		else if (worldPos.x > 837.0f && worldPos.x < 1047.0f && worldPos.y > 478.0f && worldPos.y < 510.0f) {
			const sf::Vector2u gamesize(GAMEX, GAMEY);
			const sf::Vector2u screensize = sf::Vector2u(2560, 1440);
			Engine::GetWindow().setSize(screensize);
			//set View as normal
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
		}
		else if (worldPos.x > 1101.0f && worldPos.x < 1314.0f && worldPos.y > 478.0f && worldPos.y < 510.0f) {
			const sf::Vector2u gamesize(GAMEX, GAMEY);
			const sf::Vector2u screensize = sf::Vector2u(3840, 2160);
			Engine::GetWindow().setSize(screensize);
			//set View as normal
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
		}
		else if (worldPos.x > 832.0f && worldPos.x < 897.0f && worldPos.y > 655.0f && worldPos.y < 688.0f) {
			Engine::GetWindow().create(sf::VideoMode::getFullscreenModes()[0], "Hazard Dash");
		}
		else if (worldPos.x > 1002.0f && worldPos.x < 1044.0f && worldPos.y > 652.0f && worldPos.y < 693.0f) {
			Engine::GetWindow().create(sf::VideoMode::getFullscreenModes()[0], "Hazard Dash", sf::Style::Fullscreen);
		}
		else if (worldPos.x > 832.0f && worldPos.x < 927.0f && worldPos.y > 727.0f && worldPos.y < 763.0f) {
			Engine::ChangeScene(&menu);
		}
		else if (worldPos.x > 834.0f && worldPos.x < 1192.0f && worldPos.y > 570.0f && worldPos.y < 612.0f) {
			Engine::ChangeScene(&rebindLevel);
		}
	}
	Scene::Update(dt);
}

