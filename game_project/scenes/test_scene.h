#pragma once

#include "engine.h"

class TestScene : public Scene {
private:
	void  SpawnHero(int i);
public:
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;
};
