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
		// positions        // texture coorindates
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	bool isWPressed = false, isSPressed = false, isDPressed = false, isAPressed = false;
	float cameraX = 0.0f, cameraZ = -3.0f;
	float cameraMoveSpeed = 4.0f, deltaTime = 0.12f;

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

	glEnable(GL_DEPTH_TEST);

	unsigned int vbo;
	glGenBuffers(1, &vbo);

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	Shader shader("shaders/vertex_shader.vert", "shaders/fragment_shader.frag");

	// textures
	Texture2D texture("container.jpg", GL_RGB);
	texture.setOption(GL_TEXTURE_WRAP_S, GL_REPEAT);
	texture.setOption(GL_TEXTURE_WRAP_T, GL_REPEAT);
	texture.setOption(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture.setOption(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	Texture2D texture2("awesomeface.png", GL_RGBA);

	// position vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		if (!isWPressed && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			isWPressed = true;
			cameraZ += cameraMoveSpeed * deltaTime;
		}
		else if(isWPressed && glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
			isWPressed = false;
		}

		if (!isSPressed && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			isSPressed = true;
			cameraZ -= cameraMoveSpeed * deltaTime;
		}
		else if (isSPressed && glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
			isSPressed = false;
		}

		if (!isAPressed && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			isAPressed = true;
			cameraX += cameraMoveSpeed * deltaTime;
		}
		else if (isAPressed && glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {
			isAPressed = false;
		}

		if (!isDPressed && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			isDPressed = true;
			cameraX -= cameraMoveSpeed * deltaTime;
		}
		else if (isDPressed && glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
			isDPressed = false;
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(cameraX, 0.0f, cameraZ));

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective<float>(glm::radians(45.0f), width / height, 0.1f, 100.0f);

		shader.use();
		shader.setInt("ourTexture", 0);
		
		shader.setMat4("view", glm::value_ptr(view));
		shader.setMat4("projection", glm::value_ptr(projection));

		glBindVertexArray(vao);
		for (int i = 0; i < 10; ++i) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			if ((i % 3) == 0) {
				angle = glm::radians(glfwGetTime() * 20.0f * (i+1));
				model = glm::rotate(model, angle, glm::vec3(0.5f,
					1.0f, 0.0f));
			}
			else {
				model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f,
					1.0f, 0.0f));
			}
			shader.setMat4("model", glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);

	glDeleteBuffers(1, &vbo);

	glfwTerminate();
}