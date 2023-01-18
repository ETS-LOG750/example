#include "MainWindow.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

MainWindow::MainWindow()
{
}


int MainWindow::Initialisation()
{
	// OpenGL version (usefull for imGUI and other libraries)
	const char* glsl_version = "#version 430 core";

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();

	// Request OpenGL 4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	m_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Position and colors", NULL, NULL);
	if (m_window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(m_window);
	InitializeCallback();

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return 2;
	}

	// Other openGL initialization
	// -----------------------------
	return InitializeGL();
}

void MainWindow::InitializeCallback() {
	glfwSetWindowUserPointer(m_window, reinterpret_cast<void*>(this));
	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
		MainWindow* w = reinterpret_cast<MainWindow*>(glfwGetWindowUserPointer(window));
		w->FramebufferSizeCallback(width, height);
		});
	// For other callbacks:
	// https://www.glfw.org/docs/3.3/input_guide.html
}

int MainWindow::InitializeGL()
{
	const std::string directory = SHADERS_DIR;
	bool mainShaderSuccess = true;
	m_mainShader = std::make_unique<ShaderProgram>();
	mainShaderSuccess &= m_mainShader->addShaderFromSource(GL_VERTEX_SHADER, directory + "triangles.vert");
	mainShaderSuccess &= m_mainShader->addShaderFromSource(GL_FRAGMENT_SHADER, directory + "triangles.frag");
	mainShaderSuccess &= m_mainShader->link();
	if (!mainShaderSuccess) {
		std::cerr << "Error when loading main shader\n";
		return 4;
	}
	// Get locations
	int PositionLocation = glGetAttribLocation(m_mainShader->programId(), "vPosition");
	if (PositionLocation == -1) {
		std::cerr << "Impossible to found " << "vPosition" << "\n";
		return 5;
	}
	int ColorLocation = glGetAttribLocation(m_mainShader->programId(), "vColor");
	if (ColorLocation == -1) {
		std::cerr << "Impossible to found " << "vColor" << "\n";
		return 5;
	}

	std::vector<GLfloat> pos = {
		-0.9f,  0.1f, 0.0f, // Triangle 1
		 0.0f,  0.9f, 0.0f,
		 0.9f,  0.1f, 0.0f,
		-0.1f, -0.1f, 0.0f, // Triangle 2
		-0.1f, -0.9f, 0.0f,
		-0.9f, -0.1f, 0.0f,
		 0.1f, -0.1f, 0.0f, // Triangle 3
		 0.9f, -0.9f, 0.0f,
		 0.1f, -0.9f, 0.0f
	};
	// couleur uniquement pour le premier triangle
	std::vector<GLfloat> colors = {
		0, 1, 1, 1, // s1
		1, 0, 1, 1, // s2
		1, 1, 0, 1  // s3
	};

	glGenVertexArrays(NumVAOs, m_VAOs);
	glGenBuffers(NumBuffers, m_buffers);

	//////////////////
	// Configuration VAO et VBO
	glBindVertexArray(m_VAOs[Triangles]);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[ArrayBuffer]);

	// Allocation de la mémoire
	glBufferData(GL_ARRAY_BUFFER, long(sizeof(GLfloat) * (pos.size() + colors.size())),nullptr, GL_STATIC_DRAW);
	// Upload vertex informations (vertices, color)
	glBufferSubData(GL_ARRAY_BUFFER, 0, long(sizeof(GLfloat) * pos.size()), pos.data());
	glBufferSubData(GL_ARRAY_BUFFER, 
		long(sizeof(GLfloat) * pos.size()), // Offset
		long(sizeof(GLfloat) * colors.size()), // Taille
		colors.data()); // Donnée
	// Data layout (VAO)
	// ---- Positions
	glVertexAttribPointer(PositionLocation, 3, // 3 components (XYZ) - position
		GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(PositionLocation);
	// ---- Couleurs
	glVertexAttribPointer(ColorLocation, 4, // 4 components (RGBA) - couleurs
		GL_FLOAT, GL_FALSE, 0, 
		BUFFER_OFFSET(long(sizeof(GLfloat) * pos.size()))); // offset mémoire
	// Attention: glEnableVertexAttribArray(...) n'est pas appeler pour les couleurs
	//  en effet, on veut pouvoir controler l'utilisation de cette information lors du rendu
	//  Une solution alternative serait d'utiliser 2 VAO (et optionellement 2 shaders)

	// Clean-up (optional)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Background color
	glClearColor(0.5f, 0.5f, 0.5f, 1.0);

	////////////////////
	// Blending (optional):
	// Si vous retirer le second if dans le fragment shader,
	// cela fait disparaitre le triangle
	
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}

void MainWindow::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(m_VAOs[Triangles]);
	m_mainShader->bind();

	// First triangle, vertex color
	// On doit recuperer l'attribut pour activer/desactiver
	int ColorLocation = glGetAttribLocation(m_mainShader->programId(), "vColor");
	glEnableVertexAttribArray(ColorLocation); // Activation des information des couleurs des sommets
	glDrawArrays(GL_TRIANGLES, 0, 3); // Dessin du premier triangle
	glDisableVertexAttribArray(ColorLocation); // Desactivation des information des couleurs des somments

	// Attention: quand l'attribut est desactiver, OpenGL
	// utilise la valeur par défaut: vec4(0,0,0,1)

	// Deuxieme triangle: couleur uniforme (vert)
	m_mainShader->setVec4("uColor", glm::vec4(0, 1, 0, 1));
	glDrawArrays(GL_TRIANGLES, 3, 3);

	// Troiseme triangle: couleur uniforme (rouge) -- mais transparente
	// Attention: dans le fragment shader, les couleurs transparentes sont remplacée par bleu
	m_mainShader->setVec4("uColor", glm::vec4(1, 0, 0, 0));
	glDrawArrays(GL_TRIANGLES, 6, 3);

	glFlush(); // Non necessaire
}

int MainWindow::RenderLoop()
{
	while (!glfwWindowShouldClose(m_window))
	{
		// Check inputs: Does ESC was pressed?
		if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_window, true);

		RenderScene();

		// Show rendering and get events
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	// Cleanup
	glfwDestroyWindow(m_window);
	glfwTerminate();

	return 0;
}

void MainWindow::FramebufferSizeCallback(int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
