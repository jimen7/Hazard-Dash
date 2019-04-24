#include "cmp_player_physics.h"
#include "system_physics.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>
#include <engine.h>

using namespace std;
using namespace sf;
using namespace Physics;






bool PlayerPhysicsComponent::isGrounded() const {
  auto touch = getTouching();
  const auto& pos = _body->GetPosition();
  const float halfPlrHeigt = _size.y * .5f;
  const float halfPlrWidth = _size.x * .52f;
  b2WorldManifold manifold;
  for (const auto& contact : touch) {
    contact->GetWorldManifold(&manifold);
    const int numPoints = contact->GetManifold()->pointCount;
    bool onTop = numPoints > 0;
    // If all contacts are below the player.
    for (int j = 0; j < numPoints; j++) {
      onTop &= (manifold.points[j].y < pos.y - halfPlrHeigt);
    }
    if (onTop) {
      return true;
    }
  }

  return false;
}

void PlayerPhysicsComponent::update(double dt) {

  const auto pos = _parent->getPosition();

  //Teleport to start if we fall off map.
  if (pos.y > ls::getHeight() * ls::getTileSize()) {
    teleport(ls::getTilePosition(ls::findTiles(ls::START)[0]));
  }

  if (!_AIMode) {
	  if (Joystick::isConnected(0)) {	//IF CONTROLLER IS CONNECTED USE IT


	//  sf::Joystick::update();		//NEED THIS TO KEEP INPUT OF JOYSTIC UP TO DATE

		  float xxx = sf::Joystick::getAxisPosition(0, sf::Joystick::X);	//Gets the x axis value of the Left Pad of the controller
		  //float yyy = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);	//Gets the y axis value of the Left Pad of the controller

		  //cout << xxx << endl;

		  if (xxx > 50.f || xxx < -50.f) {	//Controller inputs go grom -100 to 100, and since sticks are usually sticky we need to make sure players apply enough force.
			  // Moving Either Left or Right
			  if (xxx > 0) {
				  if (getVelocity().x < _maxVelocity.x)
					  impulse({ (float)(dt * _groundspeed), 0 });
			  }
			  else if (xxx < 0) {
				  if (getVelocity().x > -_maxVelocity.x)
					  impulse({ -(float)(dt * _groundspeed), 0 });
			  }
		  }
		  else {
			  // Dampen X axis movement
			  dampen({ 0.9f, 1.0f });
		  }
		  // Handle Jump
		  if (Joystick::isButtonPressed(0, 0)) {
			  // Handle Jump
			  _grounded = isGrounded();
			  if (_grounded) {
				  setVelocity(Vector2f(getVelocity().x, 0.f));
				  teleport(Vector2f(pos.x, pos.y - GAMEY / 720.0f * 2.0f));
				  impulse(Vector2f(0, GAMEY / 720.0f * (-6.f)));
				  // cout << Engine::getWindowSize().y << endl;
			  }

		  }
	  }
	  else {	//OTHERWISE USE KEYBOARD
		  if (Keyboard::isKeyPressed(Keyboard::Left) ||
			  Keyboard::isKeyPressed(Keyboard::Right)) {
			  // Moving Either Left or Right
			  if (Keyboard::isKeyPressed(Keyboard::Right)) {
				  if (getVelocity().x < _maxVelocity.x)
					  impulse({ (float)(dt * _groundspeed), 0 });
			  }
			  else {
				  if (getVelocity().x > -_maxVelocity.x)
					  impulse({ -(float)(dt * _groundspeed), 0 });
			  }
		  }
		  else {
			  // Dampen X axis movement
			  dampen({ 0.9f, 1.0f });
		  }
		  // Handle Jump
		  if (Keyboard::isKeyPressed(Keyboard::Up)) {
			  _grounded = isGrounded();
			  if (_grounded) {
				  setVelocity(Vector2f(getVelocity().x, 0.f));
				  teleport(Vector2f(pos.x, pos.y - GAMEY / 720.0f * 2.0f));
				  impulse(Vector2f(0, GAMEY / 720.0f * (-6.f)));
				  // cout << Engine::getWindowSize().y << endl;
			  }
		  }
	  }
  }
  else {
	  if (!_goingLeft) {
		  if (getVelocity().x < _maxVelocity.x)
			  impulse({ (float)(dt * _groundspeed), 0 });
	  }
	  else {
		  if (getVelocity().x > -_maxVelocity.x)
			  impulse({ -(float)(dt * _groundspeed), 0 });
	  }

	  if (_jumping) {
		  _grounded = isGrounded();
		  if (_grounded) {
			  setVelocity(Vector2f(getVelocity().x, 0.f));
			  teleport(Vector2f(pos.x, pos.y - GAMEY / 720.0f * 2.0f));
			  impulse(Vector2f(0, GAMEY / 720.0f * (-6.f)));
			  // cout << Engine::getWindowSize().y << endl;
		  }
	  }
  }

	



	  //Are we in air?
	  if (!_grounded) {
		  // Check to see if we have landed yet
		  _grounded = isGrounded();
		  // disable friction while jumping
		  setFriction(0.f);
	  }
	  else {
		  setFriction(0.1f);
	  }

	  // Clamp velocity.
	  auto v = getVelocity();
	  v.x = copysign(min(abs(v.x), _maxVelocity.x), v.x);
	  v.y = copysign(min(abs(v.y), _maxVelocity.y), v.y);
	  setVelocity(v);

	  PhysicsComponent::update(dt);

}

 
  

PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* p,
                                               const Vector2f& size, const std::vector <sf::Vector2ul> doors)
    : PhysicsComponent(p, true, size) {
  _doors = doors;
  _size = sv2_to_bv2(size, true);
  _maxVelocity = Vector2f(GAMEY / 720.0f * 200.f, GAMEY / 720.0f * 400.f);
  _groundspeed = GAMEY / 720.0f * 30.f;
  _grounded = false;
  _body->SetSleepingAllowed(false);
  _body->SetFixedRotation(true);
  //Bullet items have higher-res collision detection
  _body->SetBullet(true);
}
