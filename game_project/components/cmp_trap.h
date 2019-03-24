#pragma once

#include "cmp_sprite.h"
#include "cmp_physics.h"
#include "ecm.h"
#include <Box2D/Dynamics/b2Body.h>


class TrapComponent : public PhysicsComponent {
protected:
	double _damage;
	float _pushForce;

public:
	void pushBack();
	//void update(double dt) override;
	//void render() override;

	explicit TrapComponent(Entity* p, const sf::Vector2f& size);

	TrapComponent() = delete;

};