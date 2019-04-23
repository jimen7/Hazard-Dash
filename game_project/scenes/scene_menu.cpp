#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

std::shared_ptr<sf::Texture> tex;
sf::Sprite background;
shared_ptr<Entity> backEnt;

void MenuScene::Load() {
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
    auto txt1 = makeEntity();
	auto txt2 = makeEntity();
	auto txt3 = makeEntity();
	auto t1 = txt1->addComponent<TextComponent>("Start Game");
	auto t2 = txt2->addComponent<TextComponent>("Options");
	auto t3 = txt3->addComponent<TextComponent>("Exit");
	t1->setPosition(sf::Vector2f(670.0f , 250.0f));
	t1->setSize(100);
	t2->setPosition(sf::Vector2f(750.0f , 450.0f));
	t2->setSize(100);
	t3->setPosition(sf::Vector2f(820.0f, 650.0f));
	t3->setSize(100);
  };

  setLoaded(true);
  Scene::Load();
}

void MenuScene::Update(const double& dt) {

  if (sf::Keyboard::isKeyPressed(Keyboard::Space)) {
    Engine::ChangeScene(&testLevel);
  }

  if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
	  cout << "Button Pressed at " << endl;
  }

  Scene::Update(dt);
}

