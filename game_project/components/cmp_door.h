#pragma once

#include "ecm.h"


class DoorComponent : public Component {
protected:
	sf::Vector2ul _nextDoor;
	bool _enterDoor;


public:
	//static std::vector<std::shared_ptr<Entity>>* heroes_list;	//Points to the list in our main scene of the heroes
	void update(double dt) override;
	void render() override {};

	explicit DoorComponent(Entity* p, sf::Vector2f& size, sf::Vector2f& nextDoor) : Component(p), _nextDoor(nextDoor), _enterDoor(true) {}
	explicit DoorComponent(Entity* p, sf::Vector2f& size) : Component(p), _enterDoor(false) {}

	DoorComponent() = delete;

};
