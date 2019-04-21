#include "cmp_trap.h"
#include "cmp_physics.h"
#include "cmp_health.h"
#include "cmp_text.h"
#include "system_renderer.h"

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


float tileSizeTEMP =30.0f;

void TrapComponent::TrapPlayer(Entity * e, sf::Vector2f direction)
{
	e->GetCompatibleComponent<PhysicsComponent>()[0]->impulse(Vector2f(0.0f, _pushForce) - 1.0f*direction);
	//e->GetCompatibleComponent<PhysicsComponent>()[0]->impulse(_pushForce*direction);
	e->GetCompatibleComponent<HealthComponent>()[0]->ReduceHealth(_damage);
}


void TrapComponent::removeTrap(Entity *e, Component *p) {
	//e->removeComponent< e->get_components<TrapComponent>()[0] >();
	//e->removeComponent<TrapComponent>();
}

void TrapComponent::setBoolPlaced() {
	_placed = true;
}

bool TrapComponent::isPlaced() {
	return _placed;
}

void TrapComponent::update(double dt)
{


	Vector2f mousePos = Vector2f(sf::Mouse::getPosition());
	//tilePos = _parent->getPosition();


	//
	const auto dir = mousePos - _parent->getPosition();//Gets mouse potition in relation to tile's
	const auto l = sf::length(dir);

	//auto s = _parent->get_components<ShapeComponent>();

	auto c = ls::getTileAt(_parent->getPosition());
		if (l < 40.0) {
			

			_trap_colour = _selected_trap_colour;	//Set the print colour tpo be highlighted
		

			

		}
		else {
			_trap_colour = _original_trap_colour;	//Set the print colout to be the original
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




}

void TrapComponent::render() {
	_rs.setFillColor(_trap_colour);
	_rs.setSize({ tileSizeTEMP ,tileSizeTEMP });
	_rs.setPosition(_parent->getPosition() + Vector2f(-15.0f, -15.0f));
	Renderer::queue(&_rs);
}


TrapComponent::TrapComponent(Entity* p, const sf::Vector2f& size) : Component(p) {
	_damage = 0;

	//_pushForce = 1.03f;
	_pushForce = 0.03f;

}

void SpikeTrapComponent::TrapPlayer(Entity* e, sf::Vector2f direction)
{
	e->GetCompatibleComponent<PhysicsComponent>()[0]->impulse(Vector2f(0.0f, _pushForce)-1.0f*direction);
	e->GetCompatibleComponent<HealthComponent>()[0]->ReduceHealth(_damage);
}

SpikeTrapComponent::SpikeTrapComponent(Entity* p, const sf::Vector2f& size) : TrapComponent(p,size) {
	_damage = 10;
	_original_trap_colour = sf::Color::Black;
	//_pushForce = 1.03f;
	_pushForce = 60.0f;
}



void MineTrapComponent::TrapPlayer(Entity* e, sf::Vector2f direction)
{
	//e->GetCompatibleComponent<PhysicsComponent>()[0]->impulse(_pushForce*direction);
	e->GetCompatibleComponent<PhysicsComponent>()[0]->impulse(Vector2f(0.0f, _pushForce) + 0.5f*direction);
	e->GetCompatibleComponent<HealthComponent>()[0]->ReduceHealth(_damage);
}

MineTrapComponent::MineTrapComponent(Entity* p, const sf::Vector2f& size) : TrapComponent(p, size) {
	_damage = 50;
	_trap_colour = sf::Color::Blue;
	//_pushForce = 1.03f;
	_pushForce = 60.0f;
}
