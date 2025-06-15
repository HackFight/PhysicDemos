#include "simulation.hpp"
#include "app.hpp"
#include <iostream>

void Simulation::Init(App* app)
{
	m_app = app;
	for (int i = 0; i < 5; i++)
	{
		Body temp(1.0f, 1.0f, glm::vec2((i - 0.5f)*5, 0.0f));
		bodies.push_back(temp);
	}

	for (int i = 0; i < bodies.size(); i++)
	{
		if (i == bodies.size() - 1)
		{
			Constraint temp(&bodies.at(i), &bodies.at(0));
			constraints.push_back(temp);
		}
		else
		{
			Constraint temp(&bodies.at(i), &bodies.at(i+1));
			constraints.push_back(temp);
		}
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
	for (int i = 0; i < bodies.size(); i++)
	{
		Body& body = bodies.at(i);

		body.Accelerate(body.GetVelocity() * -1000.0f);
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

		//Springs
		for (int i = 0; i < constraints.size(); i++)
		{
			constraints.at(i).Satisfy();
		}
		
		//Stay in bound
		float dist = glm::length(body.position);
		if (dist > radius)
		{
			glm::vec2 n = glm::normalize(body.position);
			body.position -= body.position - n * radius;
		}

		//Mouse
		if (i == 0 && m_app->IsLeftMouseButtonDown())
		{
			body.position = m_app->GetMouseWorldPosition();
			body.SetVelocity(glm::vec2(0.0f), dt);
			body.acceleration = glm::vec2(0.0f);
		}
	}
}