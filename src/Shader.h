#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

#include <glad/glad.h>

class Shader {
	unsigned int ID;
public:
	Shader(const char* vertexPath, const char* fragmentPath);

	void use();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, float* value) const;

	~Shader();
};

#endif