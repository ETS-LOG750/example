#include "MainWindow.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "OBJLoader.h" // Inside "../shared/"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

const GLuint NumVertices = 9;

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
	m_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Transformation", NULL, NULL);
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

	// imGui: create interface
	// ---------------------------------------
	// Setup Dear ImGui context
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;// Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

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

	OBJLoader::Loader object(directory + "bunny.obj");
	if (!object.isLoaded()) {
		std::cerr << "Impossible de load the object (bunny.obj)\n";
		return 5;
	}
	// Get the first mesh
	OBJLoader::Mesh m = object.getMeshes()[0];
	const float scale = 0.5;
	const glm::vec3 offset(0.0, -0.4, 0.0);
	// -- Put all vertices inside a vector
	std::vector<GLfloat> vertices;
	m_nbVertices = 0;
	for (const OBJLoader::Vertex& v : m.vertices) {
		
		vertices.push_back(v.position[0] * scale + offset.x);
		vertices.push_back(v.position[1] * scale + offset.y);
		vertices.push_back(v.position[2] * scale + offset.z);
		vertices.push_back(v.normal[0]);
		vertices.push_back(v.normal[1]);
		vertices.push_back(v.normal[2]);
		m_nbVertices += 1;
	}

	glGenVertexArrays(NumVAOs, m_VAOs);
	glBindVertexArray(m_VAOs[Triangles]);
	glGenBuffers(NumBuffers, m_buffers);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(),
		vertices.data(), GL_STATIC_DRAW);
	// Position
	int PositionLocation = m_mainShader->attributeLocation("vPosition");
	glVertexAttribPointer(PositionLocation, 
		3, 
		GL_FLOAT,
		GL_FALSE, 
		sizeof(GLfloat) * 6, // Stride: taille d'un sommet
		BUFFER_OFFSET(0)
	);
	glEnableVertexAttribArray(PositionLocation);
	int NormalLocation = m_mainShader->attributeLocation("vNormal");
	glVertexAttribPointer(NormalLocation,
		3,
		GL_FLOAT,
		GL_TRUE,
		sizeof(GLfloat) * 6, // Stride: taille d'un sommet
		BUFFER_OFFSET(3 * sizeof(GLfloat))
	);
	glEnableVertexAttribArray(NormalLocation);

	glEnable(GL_DEPTH_TEST);

	return 0;
}

void MainWindow::RenderImgui()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//imgui 
	{
		// Astuce memoire 
		static float angle1 = 0.0;
		static float angle2 = 0.0;

		ImGui::Begin("Transformations");
		const char* items[] = { 
			"Euler", 
			"Quaternion", 
			"Interpolation Euler",
			"Interpolation Quat"
		};
		ImGui::Combo("Mode", &m_selected_mode, items, IM_ARRAYSIZE(items));

		if (m_selected_mode == 0) {
			// Euler
			ImGui::SliderAngle("Rotation x", &m_rot1.x);
			ImGui::SliderAngle("Rotation y", &m_rot1.y);
			ImGui::SliderAngle("Rotation z", &m_rot1.z);
		}
		else if (m_selected_mode == 1) {
			// Quaternion
			if (ImGui::SliderFloat("Angle", &angle1, 0, 360)) {
				m_quat1_angle = glm::radians(angle1);
			}
			if (ImGui::SliderFloat3("Axis", &m_quat1_axis[0], -1, 1)) {
				m_quat1_axis = glm::normalize(m_quat1_axis);
			}
		}
		else if (m_selected_mode == 2) {
			ImGui::Text("Rotation 1:");
			ImGui::SliderAngle("Rotation 1 x", &m_rot1.x);
			ImGui::SliderAngle("Rotation 1 y", &m_rot1.y);
			ImGui::SliderAngle("Rotation 1 z", &m_rot1.z);
			ImGui::Separator();
			ImGui::Text("Rotation 2:");
			ImGui::SliderAngle("Rotation 2 x", &m_rot2.x);
			ImGui::SliderAngle("Rotation 2 y", &m_rot2.y);
			ImGui::SliderAngle("Rotation 2 z", &m_rot2.z);
		}
		else if (m_selected_mode == 3) {
			ImGui::Text("Quaternion 1:");
			if (ImGui::SliderFloat("Angle 1", &angle1, 0, 360)) {
				m_quat1_angle = glm::radians(angle1);
			}
			if (ImGui::SliderFloat3("Axis 1", &m_quat1_axis[0], -1, 1)) {
				m_quat1_axis = glm::normalize(m_quat1_axis);
			}
			ImGui::Separator();
			ImGui::Text("Quaternion 2:");
			if (ImGui::SliderFloat("Angle 2", &angle2, 0, 360)) {
				m_quat2_angle = glm::radians(angle2);
			}
			if (ImGui::SliderFloat3("Axis 2", &m_quat2_axis[0], -1, 1)) {
				m_quat2_axis = glm::normalize(m_quat2_axis);
			}
			ImGui::Checkbox("Linear", &m_linear);
		}

		if (m_selected_mode == 2 || m_selected_mode == 3) {
			ImGui::Separator();
			ImGui::SliderFloat("Time", &m_time, 0, 1);
			ImGui::Checkbox("Animate", &m_animate);
		}
		
		if (ImGui::Button("Reset")) {
			m_rot1 = glm::vec3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f));
			m_rot2 = glm::vec3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f));
			m_quat1_angle = 0;
			m_quat1_axis = glm::vec3(0, 0, 1);
			m_quat2_angle = 0;
			m_quat2_axis = glm::vec3(0, 0, 1);
		}

		
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MainWindow::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(m_VAOs[Triangles]);
	m_mainShader->bind();

	// Ici au lieu de definir un delta
	// utilisation du temps absolu
	if (m_animate) {
		float t = glfwGetTime();
		t = std::fmod((t / 3.0), 1.0);
		m_time = t;
	}

	// Compute transformation
	glm::mat4 m = glm::mat4(1);
	if (m_selected_mode == 0) {
		m = glm::eulerAngleXYZ(m_rot1.x, m_rot1.y, m_rot1.z);
	}
	else if (m_selected_mode == 1) {
		m = glm::mat4_cast(glm::angleAxis(m_quat1_angle, m_quat1_axis));
	}
	else if (m_selected_mode == 2) {
		glm::vec3 angle = m_time * m_rot2 + (1 - m_time) * m_rot1;
		m = glm::eulerAngleXYZ(angle.x, angle.y, angle.z);
	}
	else if (m_selected_mode == 3) {
		glm::quat q1 = glm::angleAxis(m_quat1_angle, m_quat1_axis);
		glm::quat q2 = glm::angleAxis(m_quat2_angle, m_quat2_axis);
		glm::quat q;
		if (m_linear) {
			// Lerp
			q = glm::normalize(m_time * q2 + (1 - m_time) * q1); // Interpolation lineaire
		}
		else {
			// Slerp
			float theta = acos(glm::dot(q1, q2));
			float sinTheta = sin(theta);
			if (sinTheta > 0.00001) {
				q = (sin(theta * (1 - m_time)) * q1 + sin(m_time * theta) * q2) / sinTheta;
			}
		}
		// Transformation en mat4
		m = glm::mat4_cast(q);
	}


	m_mainShader->setMat4("m", m);
	m_mainShader->setMat3("mNormal", glm::inverseTranspose(glm::mat3(m)));
	glDrawArrays(GL_TRIANGLES, 0, m_nbVertices);
	

	glFlush();
}



int MainWindow::RenderLoop()
{
	while (!glfwWindowShouldClose(m_window))
	{
		// Check inputs: Does ESC was pressed?
		if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_window, true);

		RenderScene();
		RenderImgui();

		// Show rendering and get events
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

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