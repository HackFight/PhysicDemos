#include "simulation.hpp"
#include "app.hpp"
#include <iostream>

void Simulation::Init(App* app)
{
	m_app = app;
	for (int i = 0; i < 10; i++)
	{
		Body temp(1.0f, 1.0f, glm::vec2((i - 5.0f)*5, 0.0f));
		bodies.push_back(temp);
	}
}

void Simulation::Update(float deltaTime)
{
	ApplyAccelerations(deltaTime);
	ApplyConstraints(deltaTime);
	UpdatePositions(deltaTime);
}

void Simulation::Cleanup()
{
	//Clean
}

void Simulation::ApplyAccelerations(float dt)
{
	glm::vec2 toMouse;

	for (int i = 0; i < bodies.size(); i++)
	{
		Body& body = bodies.at(i);

		if (i == 0)
		{
			toMouse = glm::normalize(m_app->GetMouseWorldPosition() - body.position);
			body.Accelerate(toMouse * 2000.0f);
		}
		body.Accelerate(gravity);
	}
}

void Simulation::UpdatePositions(float dt)
{
	for (int i = 0; i < bodies.size(); i++)
	{
		Body& body = bodies.at(i);

		glm::vec2 velocity = body.position - body.lastPosition;
		body.lastPosition = body.position;
		body.position = body.position + velocity + body.acceleration * (dt * dt); //Verlet integration

		body.acceleration = {}; //RESET ACCELERATION
	}
}

void Simulation::ApplyConstraints(float dt)
{
	float radius = 90.0f;

	for (int i = 0; i < bodies.size(); i++)
	{
		Body& body = bodies.at(i);
		
		float dist = glm::length(body.position);
		
		if (dist > radius)
		{
			glm::vec2 n = glm::normalize(body.position);
			body.position -= body.position - n * radius;
		}
	}
}