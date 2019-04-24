#include "cmp_AI.h"
#include "cmp_player_physics.h"
#include <engine.h>
#include <LevelSystem.h>

using namespace std;


AIComponent::AIComponent(Entity* p) : Component(p), _goingLeft(true), _jumping (false) {
	_parent->GetCompatibleComponent<PlayerPhysicsComponent>()[0]->setAIMode(true);
}

void AIComponent::update(double dt) {
	_currentTile = LevelSystem::getTileAt(_parent->getPosition());

	if (_currentTile == 0 && _currentTile != _previousTile) {
		_jumping = false;
		_goingLeft = true;
	}
	else if (_currentTile == 2 && _currentTile != _previousTile) {
		_jumping = true;
		_goingLeft = true;
	}
	else if (_currentTile == 1 && _currentTile != _previousTile) {
		_jumping = false;
		_goingLeft = false;
	}
	else if (_currentTile == 3 && _currentTile != _previousTile) {
		_jumping = true;
		_goingLeft = false;
	}

	if (_currentTile != _previousTile) {
		_parent->GetCompatibleComponent<PlayerPhysicsComponent>()[0]->setJumping(_jumping);
		_parent->GetCompatibleComponent<PlayerPhysicsComponent>()[0]->setGoingLeft(_goingLeft);
	}
	_previousTile = _currentTile;
}