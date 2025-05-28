#pragma once
#include <glm/glm.hpp>

struct Body
{
	float radius;
	float mass;
	glm::vec2 lastPosition;
	glm::vec2 position;
	glm::vec2 acceleration;

	Body(float radius = 1.0f, float mass = 1.0f, glm::vec2 position = glm::vec2(0.0f), glm::vec2 acceleration = glm::vec2(0.0f)) :
		radius(radius), mass(mass), lastPosition(position), position(position), acceleration(acceleration)
	{ }

	void Accelerate(glm::vec2 a)
	{
		acceleration += a;
	}

	void SetVelocity(glm::vec2 v, float dt)
	{
		lastPosition = position - (v * dt);
	}

	void AddVelocity(glm::vec2 v, float dt)
	{
		lastPosition -= v * dt;
	}

	glm::vec2 GetVelocity()
	{
		return position - lastPosition;
	}
};