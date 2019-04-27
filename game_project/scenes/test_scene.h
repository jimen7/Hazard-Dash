#pragma once

#include "engine.h"

class TestScene : public Scene {
private:
	void  SpawnHero(int i);
public:
	static std::vector<std::shared_ptr<Entity>> heroes;
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;
};
