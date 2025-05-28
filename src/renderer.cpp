#include "renderer.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

#include <glm/gtc/type_ptr.hpp>


void Renderer::Clear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Clear(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Init(float* aspectRatio, float* zoom)
{
    m_aspectRatio = aspectRatio;
    m_zoom = zoom;
    shaderProgram = compileShader(RESOURCES_PATH "shaders/basic.vert", RESOURCES_PATH "shaders/circle.frag");

    projMat = glm::mat4(1.0f);
    viewMat = glm::mat4(1.0f);

    // Circle (static)
    float circleVertices[] =
    {
         0.0f,     2.0f,
         1.7321f, -1.0f,
        -1.7321f, -1.0f
    };

    glGenVertexArrays(1, &vaoCircle);
    glGenBuffers(1, &vboCircle);
    glBindVertexArray(vaoCircle);
    glBindBuffer(GL_ARRAY_BUFFER, vboCircle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
}

void Renderer::Render(const Simulation& sim)
{
    glUseProgram(shaderProgram);

    viewMat = glm::scale(glm::mat4(1.0f), glm::vec3(1 / *m_zoom));
    projMat = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / *m_aspectRatio, 1.0f, 1.0f));

    for each(Body body in sim.bodies)
    {
        drawCircle(body.position, body.radius);
    }
}

void Renderer::Cleanup()
{
    glDeleteVertexArrays(1, &vaoCircle);
    glDeleteBuffers(1, &vboCircle);
    glDeleteProgram(shaderProgram);
}

void Renderer::drawCircle(glm::vec2 center, float radius) {
    glBindVertexArray(vaoCircle);

    glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(center, 0.0f));
    modelMat = glm::scale(modelMat, glm::vec3(radius, radius, 1.0f));

    GLint modelLoc = glGetUniformLocation(shaderProgram, "uModel");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));

    GLint viewLoc = glGetUniformLocation(shaderProgram, "uView");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMat));

    GLint projLoc = glGetUniformLocation(shaderProgram, "uProj");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projMat));

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

GLuint Renderer::compileShader(const std::string& vsPath, const std::string& fsPath)
{
    std::string vsCode = loadShaderSource(vsPath);
    std::string fsCode = loadShaderSource(fsPath);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vs = vsCode.c_str();
    glShaderSource(vertexShader, 1, &vs, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fs = fsCode.c_str();
    glShaderSource(fragmentShader, 1, &fs, nullptr);
    glCompileShader(fragmentShader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

std::string Renderer::loadShaderSource(const std::string& filepath)
{
	std::ifstream file(filepath);
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}