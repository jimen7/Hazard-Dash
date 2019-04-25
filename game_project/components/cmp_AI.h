#pragma once
#include <engine.h>


class AIComponent : public Component {
private:
	bool _goingLeft;
	bool _jumping;
	int _currentTile;
	bool _stop;
	bool _choice;
	int _previousTile = 0;
	float _AIdt = 0.0f;
	float _AIdt2 = 0.0f;
	sf::Vector2f _currentPos = sf::Vector2f(-1000.f, -1000.0f);

public:
  void update(double dt) override;
  void render() override { };
  void setSwitch() { _choice = !_choice; }
  AIComponent() = delete;
  explicit AIComponent(Entity* p);
  ~AIComponent() override = default;

};
