#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

#include "Shader.h"
#include "Texture2D.h"

#include "libs/stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {

	const int width = 800;
	const int height = 600;

	float vertexData[] = {
		// positions        // colors		  // texture coorindates
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f
	};

	unsigned int indicies[] = {
		0, 1, 2,
		1, 3, 2
	};

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(width, height, "OPEN GL First Project", NULL, NULL);

	if (window == NULL) {
		std::cout << "GLFW failed to create a window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to init glad" << std::endl;
		return -1;
	}

	glViewport(0, 0, width, height);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	int nVattribs;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nVattribs);

	std::cout << "max no of vertex attributes: " << nVattribs << std::endl;

	unsigned int vbo;
	glGenBuffers(1, &vbo);

	unsigned int ebo;
	glGenBuffers(1, &ebo);

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	Shader shader("shaders/vertex_shader.vert", "shaders/fragment_shader.frag");

	// textures
	Texture2D texture("container.jpg", GL_RGB);
	texture.setOption(GL_TEXTURE_WRAP_S, GL_REPEAT);
	texture.setOption(GL_TEXTURE_WRAP_T, GL_REPEAT);
	texture.setOption(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture.setOption(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	Texture2D texture2("awesomeface.png", GL_RGBA);

	// position vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(0);

	// color vertex attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float)*3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);

	float rotAngle = 0.0f;

	glm::mat4 rotScaleMat = glm::mat4(1.0f);
	//rotScaleMat = glm::scale(rotScaleMat, glm::vec3(0.5f, 0.5f, 0.5f));
	rotScaleMat = glm::rotate(rotScaleMat, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	rotScaleMat = glm::translate(rotScaleMat, glm::vec3(0.5f, -0.5f, 0.0f));

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		shader.use();
		shader.setInt("ourTexture", 0);
		shader.setInt("secTexture", 1);
		shader.setMat4("transform", glm::value_ptr(rotScaleMat));

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

		rotScaleMat = glm::rotate(rotScaleMat, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	glDeleteVertexArrays(1, &vao);

	glDeleteBuffers(1, &vbo);

	glfwTerminate();
}