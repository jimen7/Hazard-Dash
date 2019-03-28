#include "cmp_trap.h"
#include "cmp_physics.h"

#include "system_physics.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>
#include <engine.h>


using namespace std;
using namespace sf;
using namespace Physics;

std::vector<std::shared_ptr<Entity>>* TrapComponent::heroes_list;

//void TrapComponent::getPlayers(std::vector<shared_ptr<Entity*>> _heroes) {
//	//heroes->;
////	_heroes.push_back(e);
//}


void TrapComponent::TrapPlayer(Entity * e)
{
	e->GetCompatibleComponent<PhysicsComponent>()[0]->impulse(Vector2f(_pushForce, -_pushForce));
}

void TrapComponent::update(double dt)
{
	for (auto hero_ptr : *heroes_list) {
		const auto l = sf::length((hero_ptr)->getPosition() - _parent->getPosition());
		if (l < 70.0) {
			TrapPlayer(hero_ptr.get());
		}

	}


}


TrapComponent::TrapComponent(Entity* p, const sf::Vector2f& size) : Component(p) {

	_damage = 10;
	_pushForce = 80.0f;
}

void SpikeTrapComponent::TrapPlayer(Entity * e)
{

}
