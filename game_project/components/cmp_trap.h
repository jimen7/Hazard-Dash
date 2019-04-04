#pragma once

#include "cmp_sprite.h"
#include "ecm.h"
#include <Box2D/Dynamics/b2Body.h>


class TrapComponent : public Component {
protected:
	double _damage;
	float _pushForce;


public:
	static std::vector<std::shared_ptr<Entity>>* heroes_list;	//Points to the list in our main scene of the heroes
	virtual void TrapPlayer(Entity* e, sf::Vector2f direction);
	void update(double dt) override;
	void render() override {};

	explicit TrapComponent(Entity* p, const sf::Vector2f& size);

	TrapComponent() = delete;

};


class SpikeTrapComponent : public TrapComponent {

public:
	void TrapPlayer(Entity* e, sf::Vector2f direction) override;

};