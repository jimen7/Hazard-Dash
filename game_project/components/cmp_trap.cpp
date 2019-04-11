#include "cmp_trap.h"
#include "cmp_physics.h"
#include "cmp_health.h"

#include "system_physics.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>
#include <engine.h>


using namespace std;
using namespace sf;
using namespace Physics;

//std::vector<std::shared_ptr<Entity>>* TrapComponent::heroes_list;

//void TrapComponent::getPlayers(std::vector<shared_ptr<Entity*>> _heroes) {
//	//heroes->;
////	_heroes.push_back(e);
//}


void TrapComponent::TrapPlayer(Entity * e, sf::Vector2f direction)
{
	
	e->GetCompatibleComponent<PhysicsComponent>()[0]->impulse(_pushForce*direction);
	e->GetCompatibleComponent<HealthComponent>()[0]->ReduceHealth(10);
}

void TrapComponent::update(double dt)
{
	

	if (_timer >= 0) {
		_timer -= dt;
	}
	else{
		auto collidingObjects = _parent->GetCompatibleComponent<PhysicsComponent>()[0]->getTouching();

		for (auto k : collidingObjects) {
			Entity* e1 = (Entity*)k->GetFixtureA()->GetUserData();
			Entity* e2 = (Entity*)k->GetFixtureB()->GetUserData();
			Entity* other;
			if (e2 == _parent) {
				//e1 is the thing,e2 is us
				other = e1;
			}
			else {
				//e2 is the thing,e1 is us
				other = e2;
			}
			const auto dir = other->getPosition() - _parent->getPosition();
			TrapPlayer(other, dir);
			auto a = 1;
			_timer = 0.5;
			//cout << _timer << endl;
		}
	}



	//for (auto hero_ptr : *heroes_list) {
	//	const auto dir = (hero_ptr)->getPosition() - _parent->getPosition();
	//	const auto l = sf::length(dir);
	//	if (l < 70.0) {
	//		//TrapPlayer(hero_ptr.get(), dir);
	//	}

	//}


}


TrapComponent::TrapComponent(Entity* p, const sf::Vector2f& size) : Component(p) {
	_damage = 10;

	//_pushForce = 1.03f;
	_pushForce = 0.03f;
}

void SpikeTrapComponent::TrapPlayer(Entity* e, sf::Vector2f direction)
{

}
