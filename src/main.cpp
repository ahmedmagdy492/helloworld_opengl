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

	float firstTriangleVertexData[] = {
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
	};

	float secondTriangleVertexData[] = {
		 0.0f, -0.5f, 0.0f,  // left
		 0.9f, -0.5f, 0.0f,  // right
		 0.45f, 0.5f, 0.0f
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

	unsigned int vboFirstTriangle, vboSecondTriangle;
	glGenBuffers(1, &vboFirstTriangle);
	glGenBuffers(1, &vboSecondTriangle);

	unsigned int vaoFirstTriangle, vaoSecondTriangle;
	glGenVertexArrays(1, &vaoFirstTriangle);
	glGenVertexArrays(1, &vaoSecondTriangle);
	glBindVertexArray(vaoFirstTriangle);

	glBindBuffer(GL_ARRAY_BUFFER, vboFirstTriangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangleVertexData), firstTriangleVertexData, GL_STATIC_DRAW);

	std::string vertexShaderPath = "shaders/vertex_shader.vert";
	std::string vertexShaderSrcCode = loadShader(vertexShaderPath);
	std::string fragShader1Path = "shaders/fragment_shader.frag";
	std::string fragShader1SrcCode = loadShader(fragShader1Path);
	std::string fragShader2Path = "shaders/yellow_fragment_shader.frag";
	std::string fragShader2SrcCode = loadShader(fragShader2Path);

	unsigned int vertexShader = compileShader(vertexShaderSrcCode, GL_VERTEX_SHADER);
	unsigned int fragShader1 = compileShader(fragShader1SrcCode, GL_FRAGMENT_SHADER);
	unsigned int fragShader2 = compileShader(fragShader2SrcCode, GL_FRAGMENT_SHADER);

	unsigned int shaderProgram1 = createShaderProgramAndLink(vertexShader, fragShader1);
	unsigned int shaderProgram2 = createShaderProgramAndLink(vertexShader, fragShader2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vaoSecondTriangle);
	glBindBuffer(GL_ARRAY_BUFFER, vboSecondTriangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangleVertexData), secondTriangleVertexData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shaderProgram1);

		glBindVertexArray(vaoFirstTriangle);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgram2);

		glBindVertexArray(vaoSecondTriangle);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vaoFirstTriangle);
	glDeleteVertexArrays(1, &vaoSecondTriangle);

	glDeleteBuffers(1, &vboFirstTriangle);
	glDeleteBuffers(1, &vboSecondTriangle);

	glDeleteProgram(shaderProgram1);
	glDeleteProgram(shaderProgram2);

	glfwTerminate();
}