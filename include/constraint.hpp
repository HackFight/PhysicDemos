#pragma once
#include <glm/glm.hpp>
#include "body.hpp"

class Constraint
{
public:
	Body* b1;
	Body* b2;
	float initial_length;
	float k = 100.0f;

	Constraint(Body* b1i, Body* b2i)
	{
		b1 = b1i;
		b2 = b2i;
		initial_length = glm::length(b1->position - b2->position);
	}

	void Satisfy()
	{
		glm::vec2 delta = b2->position - b1->position;
		float current_length = glm::length(delta);

		glm::vec2 x = b1->position - ((glm::normalize(-delta) * initial_length) + b2->position);
		b1->Accelerate(-k * x);

		x = b2->position - ((glm::normalize(delta) * initial_length) + b1->position);
		b2->Accelerate(-k * x);
	}
};