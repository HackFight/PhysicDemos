#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "renderer.hpp"
#include "simulation.hpp"

class App
{
public:
	void Run();
	glm::vec2 GetMouseWorldPosition();
	bool IsLeftMouseButtonDown();
	bool IsRightMouseButtonDown();

private:
	int m_width = 512;
	int m_height = 512;

	float m_aspectRatio = 1.0f;
	float m_zoom = 100.0f;

	GLFWwindow* m_window;
};