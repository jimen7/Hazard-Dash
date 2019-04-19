#pragma once

#include "cmp_sprite.h"
#include "ecm.h"
#include <Box2D/Dynamics/b2Body.h>


class TrapComponent : public Component {
protected:
	//float _tileSize = (Engine::getWindowSize().y / 720.0f) * 20.0f;
	bool _placed = false;
	double _damage;
	float _pushForce;
	float _timer=0;
	sf::Color _trap_colour = sf::Color::White;
	int _trapType=0;  //0 is deafult for testing(Spikes), 1 is OP spikes  for nopw 

public:
	//static std::vector<std::shared_ptr<Entity>>* heroes_list;	//Points to the list in our main scene of the heroes
	virtual void TrapPlayer(Entity* e, sf::Vector2f direction);
	void update(double dt) override;
	void render() override {};
	void placeTrap();
	//void removeTrap();

	explicit TrapComponent(Entity* p, const sf::Vector2f& size);

	TrapComponent() = delete;

};


class SpikeTrapComponent : public TrapComponent {
private:
	//string description = "Spikes";

public:
	void TrapPlayer(Entity* e, sf::Vector2f direction) override;
	explicit SpikeTrapComponent(Entity* p, const sf::Vector2f& size);

};

class MineTrapComponent : public TrapComponent {
private:
	//string description = "Mine";

public:
	void TrapPlayer(Entity* e, sf::Vector2f direction) override;
	explicit MineTrapComponent(Entity* p, const sf::Vector2f& size);

};