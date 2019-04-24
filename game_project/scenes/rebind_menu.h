#pragma once

#include "engine.h"

class RebindScene : public Scene {
public:
	RebindScene() = default;
	~RebindScene() override = default;

	void Load() override;

	void Update(const double& dt) override;
};
