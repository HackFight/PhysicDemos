#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>
#include <vector>
#include "simulation.hpp"

class Renderer
{
public:
	void Clear();
	void Clear(float r, float g, float b);

	void Init(float* aspectRatio, float* zoom);
	void Render(const Simulation& sim);
	void Cleanup();

private:
	GLuint shaderProgram;
	GLuint vaoCircle, vboCircle;

	float* m_aspectRatio;
	float* m_zoom;

	glm::mat4 viewMat = glm::mat4(1.0f);
	glm::mat4 projMat = glm::mat4(1.0f);

	void drawCircle(glm::vec2 center, float radius);
	GLuint compileShader(const std::string& vertexSrc, const std::string& fragmentSrc);
	std::string loadShaderSource(const std::string& filepath);
};
