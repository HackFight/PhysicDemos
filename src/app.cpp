#include "app.hpp"

void App::Run()
{
	// INIT GLFW
	if (!glfwInit())
	{
		std::cerr << "Failed to initialise GLFW!\n";
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, 0);

	//CREATE WINDOW
	m_window = glfwCreateWindow(
		m_width, m_height, "Test window", NULL, NULL);

	if (m_window == NULL) //Checking that the window has been sucessfully created
	{
		std::cerr << "Failed to create a GLFW window!\n";
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_window);
	gladLoadGL();


	Renderer renderer;
	Simulation sim;

	renderer.Init(&m_aspectRatio, &m_zoom);
	sim.Init(this);

	float lastTime = glfwGetTime();
	float accumulator = 0.0f;
	const float fixedDelta = 1.0f / 60.0f;


	//MAIN LOOP
	while (!glfwWindowShouldClose(m_window)) //Will run until the window is closed
	{
		glfwGetFramebufferSize(m_window, &m_width, &m_height);
		glViewport(0, 0, m_width, m_height);
		m_aspectRatio = float(m_width) / float(m_height);

		float currentTime = glfwGetTime();
		float frameTime = currentTime - lastTime;
		lastTime = currentTime;

		frameTime = std::min(frameTime, 0.1f);
		accumulator += frameTime;

		while (accumulator >= fixedDelta)
		{
			for (int i = 0; i < 8; i++)
			{
				sim.Update(fixedDelta / 8);
			}
			accumulator -= fixedDelta;
		}

		glfwPollEvents();        // Take care of all GLFW events

		renderer.Clear();
		renderer.Render(sim);


		glfwSwapBuffers(m_window); // Swap the back buffer with the front buffer
	}

	//Cleanup
	sim.Cleanup();
	renderer.Cleanup();
	glfwDestroyWindow(m_window);  //Cleaning everything up
	glfwTerminate();
}

glm::vec2 App::GetMouseWorldPosition()
{
	double raw_xpos, raw_ypos;
	float xpos, ypos;
	glfwGetCursorPos(m_window, &raw_xpos, &raw_ypos);

	xpos = (raw_xpos / m_width);			//Convert to 0-1 range
	ypos = (raw_ypos / m_height);

	xpos = 2 * xpos - 1;					//Convert to -1 - 1 range
	ypos = 2 * ypos - 1;

	ypos *= -1;								//Invert y axis

	xpos *= m_zoom * m_aspectRatio;
	ypos *= m_zoom;

	return glm::vec2(xpos, ypos);
}