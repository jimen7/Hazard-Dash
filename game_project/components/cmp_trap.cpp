#include "cmp_trap.h"

#include "system_physics.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>
#include <engine.h>


using namespace std;
using namespace sf;
using namespace Physics;

void TrapComponent::pushBack() {
	if (ls::getTileAt(_parent->getPosition()) == ls::TRAP) {
		impulse(Vector2f(_pushForce,_pushForce));
	}


}


TrapComponent::TrapComponent(Entity* p, const sf::Vector2f& size) : PhysicsComponent(p, true, size) {
	_damage = 10;
	_pushForce = 80.0f;
	_body->SetSleepingAllowed(false);
	_body->SetFixedRotation(true);
	//Bullet items have higher-res collision detection
	_body->SetBullet(true);
}

