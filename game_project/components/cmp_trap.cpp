#include "cmp_trap.h"
#include "cmp_physics.h"
#include "cmp_health.h"

#include "system_physics.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <engine.h>


using namespace std;
using namespace sf;
using namespace Physics;
// tileSize;
//Vector2f tilePos;




void TrapComponent::TrapPlayer(Entity * e, sf::Vector2f direction)
{
	
	e->GetCompatibleComponent<PhysicsComponent>()[0]->impulse(_pushForce*direction);
	e->GetCompatibleComponent<HealthComponent>()[0]->ReduceHealth(_damage);
}

void TrapComponent::update(double dt)
{


	Vector2f mousePos = Vector2f(sf::Mouse::getPosition());
	//tilePos = _parent->getPosition();


	//
	const auto dir = mousePos - _parent->getPosition();//Gets mouse potition in relation to tile's
	const auto l = sf::length(dir);

	auto s = _parent->get_components<ShapeComponent>();

	auto c = ls::getTileAt(_parent->getPosition());
		if (l < 70.0) {
			s[0]->getShape().setFillColor(Color::Blue);	//If this line is removed, current colour is changed, whichg means that the colour is changing but not drawn on the screen

			auto currentColor = s[0]->getShape().getFillColor();
			int i = 9;		//DEBBUG POINT
			}
		else {
			//s[0]->getShape().setFillColor(Color::Black);
		}

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
	e->GetCompatibleComponent<PhysicsComponent>()[0]->impulse(_pushForce*direction);
	e->GetCompatibleComponent<HealthComponent>()[0]->ReduceHealth(_damage);
}

SpikeTrapComponent::SpikeTrapComponent(Entity* p, const sf::Vector2f& size) : TrapComponent(p,size) {
	_damage = 10;

	//_pushForce = 1.03f;
	_pushForce = 1.03f;
}
