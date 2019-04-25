#include "cmp_trap.h"
#include "cmp_physics.h"
#include "cmp_health.h"
#include "cmp_text.h"
#include "system_renderer.h"
#include "cmp_sprite.h"

#include "system_physics.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <engine.h>
#include <thread>
#include <chrono>


using namespace std;
using namespace sf;
using namespace Physics;

using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

// tileSize;
//Vector2f tilePos;


float tileSizeTEMP = GAMEX / 64;

void TrapComponent::TrapPlayer(Entity * e, sf::Vector2f direction)
{
	//e->GetCompatibleComponent<PhysicsComponent>()[0]->impulse(Vector2f(0.0f, _pushForce) - 1.0f*direction);
	//e->GetCompatibleComponent<PhysicsComponent>()[0]->impulse(_pushForce*direction);
	e->GetCompatibleComponent<HealthComponent>()[0]->ReduceHealth(_damage);
}


void TrapComponent::removeTrap(Entity *e, Component *p) {
	//e->removeComponent< e->get_components<TrapComponent>()[0] >();
	//e->removeComponent<TrapComponent>(Vector2f(tileSizeTEMP, tileSizeTEMP));
}

void TrapComponent::setBoolPlaced() {
	_placed = true;
}

bool TrapComponent::isPlaced() {
	return _placed;
}

void TrapComponent::update(double dt)
{


	// get the current mouse position in the window
	const sf::Vector2i pixelPos = sf::Mouse::getPosition(Engine::GetWindow());
	// convert it to world coordinate, because we scale in the render from 1080p to the target resolution
	const sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(pixelPos);
	const auto dir = Vector2f(worldPos) - _parent->getPosition();//Gets mouse potition in relation to tile's
	const auto l = sf::length(dir);

	//cout << "Mouse Position:(" << mousePos << ")" << endl;
	//cout << "Trap Position:(" << _parent->getPosition() << ")" << endl;


	if (l<25.0) {
			_trap_colour = _selected_trap_colour;	//Set the print colour tpo be highlighted
	}
	else {
			_trap_colour = _original_trap_colour;	//Set the print colout to be the original
	}



	if (_parent->GetCompatibleComponent<PhysicsComponent>().size() != 0) {		//Check if player has physics comp[onents. Physics Components are only added if it is an advanced trap, as player can walk through empty traps.
		if (_timer >= 0) {
			_timer -= dt;
		}
		else {

			if (_damage == 20) {
				if (l < 25.0 && sf::Mouse::isButtonPressed(Engine::_Keysss["Click"].myMouseButton)) {//Spikes only activate when pressed
						_parent->setPosition(_parent->getPosition() + Vector2f(0, -tileSizeTEMP));
						_spikeActivated = true;
				}
			}
			}

			auto collidingObjects = _parent->GetCompatibleComponent<PhysicsComponent>()[0]->getTouching();//Gets any colliding objects with the trap, improved from checking against every hero


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
				if (_damage != 20) {//Check its not spikes
					TrapPlayer(other, dir);
				}
				else {
					if (_spikeActivated) {
						TrapPlayer(other, dir);
						_spikeActivated = false;
					}
					//_parent->setPosition(_parent->getPosition() + Vector2f(0, -tileSizeTEMP));
				}
				auto a = 1;
				_timer = 0.5;
			}
	}

	
	



}

void TrapComponent::render() {
	/*_rs.setFillColor(_trap_colour);
	_rs.setSize({ tileSizeTEMP ,tileSizeTEMP });
	_rs.setPosition(_parent->getPosition() + Vector2f(-tileSizeTEMP/ DIVIDER, -tileSizeTEMP / DIVIDER));
	Renderer::queue(&_rs);*/
}


TrapComponent::TrapComponent(Entity* p, const sf::Vector2f& size) : Component(p) {

	auto s = _parent->GetCompatibleComponent<SpriteComponent>()[0];

	_trapSpritesheet = std::make_shared<sf::Texture>();

	_trapSpritesheet->loadFromFile("res/Sprites/Esquire2.png");
	if (!_trapSpritesheet->loadFromFile("res/Sprites/Esquire2.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}
	s->setTexure(_trapSpritesheet);
	//s->setTextureRect(sf::IntRect(0, 0, 40, 40));
	s->setTextureRect(sf::IntRect(0, 128, 32, 32));
	_damage = 0;
	//_pushForce = 1.03f;
	_pushForce = 0.03f;

}

void SpikeTrapComponent::TrapPlayer(Entity* e, sf::Vector2f direction)
{

	e->setPosition(e->getPosition()+Vector2f(0,tileSizeTEMP));
	e->GetCompatibleComponent<PhysicsComponent>()[0]->impulse(Vector2f(0.0f, _pushForce) - 1.0f*direction);
	e->GetCompatibleComponent<HealthComponent>()[0]->ReduceHealth(_damage);
	//sleep_for(std::chrono::seconds(2));
	
}

SpikeTrapComponent::SpikeTrapComponent(Entity* p, const sf::Vector2f& size) : TrapComponent(p,size) {
	
	//Sprite Stuff
	auto s = _parent->GetCompatibleComponent<SpriteComponent>()[0];

	_trapSpritesheet = std::make_shared<sf::Texture>();

	_trapSpritesheet->loadFromFile("res/Sprites/traps/spikes_resized.png");
	if (!_trapSpritesheet->loadFromFile("res/Sprites/traps/spikes_resized.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}
	s->setTexure(_trapSpritesheet);
	//s->setTextureRect(sf::IntRect(0, 0, 40, 40));
	s->setTextureRect(sf::IntRect(0, 0, 32, 32));

	_damage = 20;
	_original_trap_colour = sf::Color::Blue;
	//_pushForce = 1.03f;
	_pushForce = 60.0f;
	_placed = true;
	//p->addComponent<TrapComponent>(Vector2f(tileSizeTEMP, tileSizeTEMP));
}



void MineTrapComponent::TrapPlayer(Entity* e, sf::Vector2f direction)
{
	//e->GetCompatibleComponent<PhysicsComponent>()[0]->impulse(_pushForce*direction);
	e->GetCompatibleComponent<PhysicsComponent>()[0]->impulse(Vector2f(0.0f, _pushForce) + 0.5f*direction);
	e->GetCompatibleComponent<HealthComponent>()[0]->ReduceHealth(_damage);
}

MineTrapComponent::MineTrapComponent(Entity* p, const sf::Vector2f& size) : TrapComponent(p, size) {

	auto s = _parent->GetCompatibleComponent<SpriteComponent>()[0];

	_trapSpritesheet = std::make_shared<sf::Texture>();

	_trapSpritesheet->loadFromFile("res/Sprites/traps/mine/mine_1_correct.png");
	if (!_trapSpritesheet->loadFromFile("res/Sprites/traps/mine/mine_1_correct.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}
	s->setTexure(_trapSpritesheet);
	//s->setTextureRect(sf::IntRect(0, 0, 40, 40));
	s->setTextureRect(sf::IntRect(0, 0, 32, 32));

	
	_damage = 50;
	_original_trap_colour = sf::Color::Yellow;
	//_pushForce = 1.03f;
	_pushForce = 60.0f;
	_placed = true;
	//p->addComponent<TrapComponent>(Vector2f(tileSizeTEMP, tileSizeTEMP));
}
