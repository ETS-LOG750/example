#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <memory>
#include <glm/gtx/euler_angles.hpp>

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

private:
	// Initialize GLFW callbacks
	void InitializeCallback();
	// Intiialize OpenGL objects (shaders, ...)
	int InitializeGL();

	// Rendering scene (OpenGL)
	void RenderScene();
	// Rendering interface ImGUI
	void RenderImgui();

private:
	// settings
	const unsigned int SCR_WIDTH = 900;
	const unsigned int SCR_HEIGHT = 720;
	GLFWwindow* m_window = nullptr;

	// Transformations
	// - angles euler (x, y, z)
	glm::vec3 m_rot;
	// - scale and translation
	glm::vec3 m_scale,m_translate;
	
	// Camera
	glm::vec3 m_eye, m_at, m_up;
	glm::mat4 m_proj;

	enum VAO_IDs { Triangles, NumVAOs };
	enum Buffer_IDs { ArrayBuffer, NumBuffers };

	GLuint m_VAOs[NumVAOs];
	GLuint m_buffers[NumBuffers];

	std::vector<GLfloat> m_vertices; // Array holding vertices
	std::vector<GLfloat> m_normals; // Array holding normals
	GLint m_numCoordinatesPerVertices; // Number of coordinates per vertex in the m_vertices array

	std::unique_ptr<ShaderProgram> m_mainShader = nullptr;
};
