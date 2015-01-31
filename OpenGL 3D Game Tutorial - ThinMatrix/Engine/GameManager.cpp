#include <iostream>
#include <GL\glew.h>
#include "GameManager.h"
#include "../RenderEngine/Loader.h"
#include "../RenderEngine/Renderer.h"
#include "../Models/RawModel.h"
#include "../Models/TexturedModel.h"
#include "../Textures/ModelTexture.h"
#include "../Shaders/StaticShader.h"

GameManager::GameManager()
{
	// Initialized GLFW
	if (glfwInit())
	{
		std::cout << "GLFW initialized successfully" << std::endl;
		// Set minimum OpenGL version and options
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GL_TRUE);

		// Create the display manager (pointers must be deleted)
		m_displayManager = new DisplayManager(1280, 720, "ThinMatrix OpenGL Game");

		// Initialize glew using experimental(new)
		glewExperimental = true;
		GLenum status = glewInit();
		if (status == GLEW_OK)
		{
			std::cout << "GLEW initialized successfully" << std::endl;
		}
		else
		{
			std::cerr << "ERROR: GLEW failed to initialize \n" << glewGetErrorString(status) << std::endl;
		}
	}
	else
	{
		std::cerr << "ERROR: GLFW failed to initialize" << std::endl;
	}
}


GameManager::~GameManager()
{
	// Delete the display and clean up GLFW
	delete m_displayManager;
	glfwTerminate();
}


void GameManager::Start()
{
	std::cout << "Game loop is now running" << std::endl;

	Loader loader;
	Renderer renderer;

	StaticShader staticShader("basicShader");

	// START temporary model data
	float vertices[] = {
		-0.5f,  0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f
	};

	int indices[] = {
		0, 1, 3,
		3, 1, 2
	};

	float texCoords[] = {
		0, 0,
		0, 1,
		1, 1,
		1, 0
	};

	RawModel model = loader.LoadToVAO(vertices, indices, texCoords,
		sizeof(vertices) / sizeof(vertices[0]),
		sizeof(indices) / sizeof(indices[0]),
		sizeof(texCoords) / sizeof(texCoords[0]));
	ModelTexture texture(loader.LoadTexture("image"));
	TexturedModel texturedModel(model, texture);
	// END temporary model data

	// Start the game loop
	while (m_displayManager->IsWindowOpen())
	{
		renderer.Prepare();
		staticShader.Use();
		renderer.Render(&texturedModel);
		staticShader.UnUse();

		m_displayManager->UpdateDisplay();
	}
}