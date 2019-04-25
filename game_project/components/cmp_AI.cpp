#include "cmp_AI.h"
#include "cmp_player_physics.h"
#include <engine.h>
#include <LevelSystem.h>

using namespace std;


AIComponent::AIComponent(Entity* p) : Component(p), _goingLeft(true), _jumping (false) {
	_parent->GetCompatibleComponent<PlayerPhysicsComponent>()[0]->setAIMode(true);
	srand(time(0));
	_choice = rand() & 1;
}

void AIComponent::update(double dt) {
	_AIdt += dt;
	_AIdt2 += dt;
	_currentTile = LevelSystem::getTileAt(_parent->getPosition());

	if (_currentTile == 0 && _currentTile != _previousTile) {
		_jumping = false;
		_goingLeft = true;
		_stop = false;
	}
	else if (_currentTile == 2 && _currentTile != _previousTile) {
		_jumping = true;
		_goingLeft = true;
		_stop = false;
	}
	else if (_currentTile == 1 && _currentTile != _previousTile) {
		_jumping = false;
		_goingLeft = false;
		_stop = false;
	}
	else if (_currentTile == 3 && _currentTile != _previousTile) {
		_jumping = true;
		_goingLeft = false;
		_stop = false;
	}
	else if (_currentTile == 4 && _currentTile != _previousTile) {
		if (_choice) {
			_jumping = false;
			_goingLeft = true;
			_stop = false;
		}
		else {
			_jumping = true;
			_goingLeft = true;
			_stop = false;
		}
	}
	else if (_currentTile == 5 && _currentTile != _previousTile) {
		_stop = true;
		_jumping = true;
	}

	// For when the AI gets stuck
	if (_AIdt > 0.2f) {
		_AIdt = 0.0f;

		if (_currentPos == _parent->getPosition()) {
			_currentTile = 6;
			_jumping = !_jumping;
			_goingLeft = !_goingLeft;
		}

		_currentPos = _parent->getPosition();
	}

	if (_AIdt2 > 4.0f) {
		_AIdt2 = 0.0f;
		if (_currentPos == _parent->getPosition()) {
			if (_currentPos.x  < 1000)
				_parent->GetCompatibleComponent<PlayerPhysicsComponent>()[0]->teleport(_parent->getPosition() - sf::Vector2f(10,0));
			else
				_parent->GetCompatibleComponent<PlayerPhysicsComponent>()[0]->teleport(_parent->getPosition() - sf::Vector2f(-10, 0));
		}

	}

	if (_currentTile != _previousTile) {
		_parent->GetCompatibleComponent<PlayerPhysicsComponent>()[0]->setJumping(_jumping);
		_parent->GetCompatibleComponent<PlayerPhysicsComponent>()[0]->setGoingLeft(_goingLeft);
		_parent->GetCompatibleComponent<PlayerPhysicsComponent>()[0]->setStop(_stop);
	}
	_previousTile = _currentTile;
}