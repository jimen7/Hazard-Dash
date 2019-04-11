#pragma once
#include "cmp_physics.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <ecm.h>

class HealthComponent : public Component {
protected:
	float _health;
	bool _isAlive;
	sf::RectangleShape _rs;
public:
	//static std::vector<std::shared_ptr<Entity>>* heroes_list;	//Points to the list in our main scene of the heroes
	void update(double dt) override;
	void render() override;

	void ReduceHealth(float amount);

	explicit HealthComponent(Entity* p,  float i);
	HealthComponent() = delete;
};
