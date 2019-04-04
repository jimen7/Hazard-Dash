#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class HealthComponent : public Component {
protected:
	float _health;
	bool _isAlive;

public:
	static std::vector<std::shared_ptr<Entity>>* heroes_list;	//Points to the list in our main scene of the heroes
	void update(double dt) override;
	void render() override {}

	void ReduceHealth(int amount);

	explicit HealthComponent(Entity* p, const sf::Vector2f& size, int i);
	HealthComponent() = delete;
};
