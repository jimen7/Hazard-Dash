#include "cmp_health.h"
#include "cmp_physics.h"
#include "../../engine/lib_ecm/ecm.h"
#include "system_renderer.h"
#include "system_physics.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>
#include <engine.h>


using namespace std;
using namespace sf;
using namespace Physics;

float tileSizeHealth = GAMEX / 64;

//std::vector<std::shared_ptr<Entity>>* HealthComponent::heroes_list;

//void TrapComponent::getPlayers(std::vector<shared_ptr<Entity*>> _heroes) {
//	//heroes->;
////	_heroes.push_back(e);
//}


void HealthComponent::ReduceHealth(float amount) {
	_health -= amount;
	//cout << _health << endl;
}




void HealthComponent::update(double dt)
{
		if (_health <= 0) {
			_parent->setForDelete();
			_isAlive = false;
		}
}

float HealthComponent::getHealth() {
	return _health;
}

sf::Color ColLerp(const sf::Color& c1, const  sf::Color& c2, float a) {
	sf::Color ret = sf::Color::White;
	ret.r = (1 - a)*c1.r + a * c2.r;
	ret.g = (1 - a)*c1.g + a * c2.g;
	ret.b = (1 - a)*c1.b + a * c2.b;
	return ret;
}


void HealthComponent::render()
{

	auto healthPercent = _health / _originalHealth;
	auto r = sf::Color::Red;
	_rs.setFillColor(ColLerp(sf::Color::Red,sf::Color::Green,healthPercent));

	_rs.setSize({32,8});
	_rs.setPosition(_parent->getPosition() + Vector2f(-tileSizeHealth/DIVIDER, -tileSizeHealth/2));
	Renderer::queue(&_rs);
}


HealthComponent::HealthComponent(Entity* p, float health_amount) : Component(p) {
	_health = health_amount;
	_originalHealth = health_amount;
}

