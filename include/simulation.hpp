#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "body.hpp"

class App;

class Simulation
{
public:
	void Init(App* app);
	void Update(float deltaTime);
	void Cleanup();

	//Particle
	std::vector<Body> bodies;

	glm::vec2 gravity = glm::vec2(0.0f, -1000.0f);

private:
	App* m_app;

	void ApplyAccelerations(float dt);
	void ApplyConstraints(float dt);
	void UpdatePositions(float dt);
};