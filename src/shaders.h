#pragma once
#include <iostream>
#include <string>
#include <unordered_map>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct SHADER {
	std::string vertexShader;
	std::string fragmentShader;
};

class Shader7 {
	SHADER sd;
	unsigned int ID;
	std::unordered_map<std::string, int> uniformLocationSet;

	unsigned int compileShader(unsigned int shaderType, const std::string& shaderSource);
	void createShader(const std::string& vertexShader, const std::string& fragmentShader);
	SHADER loadFromFile(const std::string source);
	int GetUniformLocation(const std::string& name);

public:
	Shader7(const std::string shaderPath);
	~Shader7();

	void bind();
	void unbind();
	
	unsigned int getID();

	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float x, float y, float z, float w);
	void SetUniformMat4f(const std::string& name, const glm::mat4 matrix);
	void SetUniform3f(const std::string& name, float x, float y, float z);
	void SetUniform3f(const std::string& name, glm::vec3 v);

	void printVertexShader();
	void printFragmentShader();
};