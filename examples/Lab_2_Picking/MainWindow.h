#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <memory>

#include "ShaderProgram.h"

class MainWindow
{
public:
	MainWindow();

	// Main functions (initialization, run)
	int Initialisation();
	int RenderLoop();

	// Callback to intersept GLFW calls
	void FramebufferSizeCallback(int width, int height);
	void MouseButtonCallback(int button, int action, int mods);

private:
	// Initialize GLFW callbacks
	void InitializeCallback();
	// Intiialize OpenGL objects (shaders, ...)
	int InitializeGL();
	// Load spiral geometry (0 = success)
	int InitGeometrySpiral();

	// Rendering scene (OpenGL)
	void RenderScene();
	// Rendering interface ImGUI
	void RenderImgui();
	
	// Perform selection on the object
	void PerformSelection(int x, int y);

private:
	// settings
	unsigned int m_windowWidth = 1200;
	unsigned int m_windowHeight = 800;

	// GLFW Window
	GLFWwindow* m_window = nullptr;

	// VAOs and VBOs
	enum VAO_IDs { VAO_Spiral, VAO_SpiralSelected, VAO_SpiralPicking, VAO_Ray, NumVAOs };
	enum Buffer_IDs { VBO_Spiral, VBO_Ray, NumBuffers };

	GLuint m_VAOs[NumVAOs];
	GLuint m_buffers[NumBuffers];

	// Camera
	glm::mat4 m_projectionMatrix = glm::mat4(1.0);
	glm::mat4 m_modelViewMatrix = glm::mat4(1.0);
	
	// Render shaders & locations
	std::unique_ptr<ShaderProgram> m_mainShader = nullptr;
	std::unique_ptr<ShaderProgram> m_pickingShader = nullptr;

	// Picking parameters
	int m_selectedSpiral = -1;

};
