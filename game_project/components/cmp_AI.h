#pragma once
#include <engine.h>


class AIComponent : public Component {
private:
	bool _goingLeft;
	bool _jumping;
	int _currentTile;
	int _previousTile = 0;

public:
  void update(double dt) override;
  void render() override { };
  AIComponent() = delete;
  explicit AIComponent(Entity* p);
  ~AIComponent() override = default;

};
