#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	std::string vertexShaderSrc;
	std::string fragmentShaderSrc;
	std::ifstream vertexIfStream;
	std::ifstream fragmentIfStream;

	vertexIfStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentIfStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vertexIfStream.open(vertexPath);
		fragmentIfStream.open(fragmentPath);
		std::stringstream vertexStrStream, fragmentStrStream;

		vertexStrStream << vertexIfStream.rdbuf();
		fragmentStrStream << fragmentIfStream.rdbuf();

		vertexIfStream.close();
		fragmentIfStream.close();

		vertexShaderSrc = vertexStrStream.str();
		fragmentShaderSrc = fragmentStrStream.str();

		const char* vShaderSrc = vertexShaderSrc.c_str();
		const char* fShaderSrc = fragmentShaderSrc.c_str();

		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

	 	vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderSrc, NULL);
		glCompileShader(vertex);

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
				infoLog << std::endl;
		}

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderSrc, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
				infoLog << std::endl;
		}

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);

		glLinkProgram(ID);
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
				infoLog << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const {
	int location = glGetUniformLocation(ID, name.c_str());
	glUniform1i(location, (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
	int location = glGetUniformLocation(ID, name.c_str());
	glUniform1i(location, value);
}

void Shader::setFloat(const std::string& name, float value) const {
	int location = glGetUniformLocation(ID, name.c_str());
	glUniform1f(location, value);
}

void Shader::setMat4(const std::string& name, float* value) const {
	int location = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, value);
}

Shader::~Shader() {
	glDeleteProgram(ID);
}