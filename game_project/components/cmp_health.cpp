#include "cmp_health.h"
#include "cmp_physics.h"

#include "system_physics.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>
#include <engine.h>


using namespace std;
using namespace sf;
using namespace Physics;

std::vector<std::shared_ptr<Entity>>* HealthComponent::heroes_list;

//void TrapComponent::getPlayers(std::vector<shared_ptr<Entity*>> _heroes) {
//	//heroes->;
////	_heroes.push_back(e);
//}


void HealthComponent::ReduceHealth(int amount) {
	_health -= amount;
}


void HealthComponent::update(double dt)
{
	for (auto hero_ptr : *heroes_list) {
		//if ()
	}


}


HealthComponent::HealthComponent(Entity* p, const sf::Vector2f& size, int health_amount) : Component(p) {
	_health = health_amount;
}
