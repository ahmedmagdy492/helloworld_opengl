#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

std::string loadShader(std::string& path) {
	std::fstream fs;

	fs.open(path, std::ios_base::in);

	if (!fs.is_open()) {
		std::cout << "Failed to load shader file " << path << std::endl;
		return "";
	}
	std::stringstream strStream;

	strStream << fs.rdbuf();

	fs.close();

	return strStream.str();
}

unsigned int compileShader(const std::string& shaderSrcCode, GLenum shaderType) {
	unsigned int shaderObj;
	shaderObj = glCreateShader(shaderType);

	GLchar* const* shaderSrcCodeCStr = (GLchar* const*)shaderSrcCode.c_str();
	glShaderSource(shaderObj, 1, (GLchar* const*)&shaderSrcCodeCStr, NULL);
	glCompileShader(shaderObj);

	int success;
	char infoLog[512];
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shaderObj, 512, NULL, infoLog);
		std::cout << "Failed to compile shader: " << infoLog << std::endl;
	}

	return shaderObj;
}

unsigned int createShaderProgramAndLink(unsigned int vertexShader, unsigned int fragShader) {
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Error linking the program to open gl context: " << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	return shaderProgram;
}

int main() {

	const int width = 800;
	const int height = 600;

	float vertexData[] = {
		-0.5f,  0.5f, 0.0f,  
		 0.5f,  0.5f, 0.0f,  
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	unsigned int indicies[] = {
		0, 1, 2,
		0, 3, 2
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

	std::string vertexShaderPath = "shaders/vertex_shader.vert";
	std::string vertexShaderSrcCode = loadShader(vertexShaderPath);
	std::string fragShaderPath = "shaders/fragment_shader.frag";
	std::string fragShaderSrcCode = loadShader(fragShaderPath);

	unsigned int vertexShader = compileShader(vertexShaderSrcCode, GL_VERTEX_SHADER);
	unsigned int fragShader = compileShader(fragShaderSrcCode, GL_FRAGMENT_SHADER);

	unsigned int shaderProgram = createShaderProgramAndLink(vertexShader, fragShader);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);

	glDeleteBuffers(1, &vbo);

	glDeleteProgram(shaderProgram);

	glfwTerminate();
}