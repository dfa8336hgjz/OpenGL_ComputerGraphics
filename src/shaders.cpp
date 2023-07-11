#include "shaders.h"
#include <fstream>
#include <sstream>

Shader7::Shader7(const std::string shaderPath) : ID(0)
{
	sd = loadFromFile(shaderPath);
	createShader(sd.vertexShader, sd.fragmentShader);
}

Shader7::~Shader7()
{
	glDeleteProgram(ID);
}

void Shader7::bind()
{
	glUseProgram(ID);
}

void Shader7::unbind()
{
	glUseProgram(0);
}

unsigned int Shader7::getID()
{
	return ID;
}

unsigned int Shader7::compileShader(unsigned int shaderType, const std::string& shaderSource)
{
	unsigned int Shader = glCreateShader(shaderType);
	const char* sds = shaderSource.c_str();
	glShaderSource(Shader, 1, &sds, nullptr);
	glCompileShader(Shader);

	int result;
	glGetShaderiv(ID, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(ID, length, &length, message);

		std::cout << "Failed to compile " << (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(ID);
		return 0;
	}

	return Shader;
}

void Shader7::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int vShader = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fShader = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
	ID = glCreateProgram();
	glAttachShader(ID, vShader);
	glAttachShader(ID, fShader);
	glLinkProgram(ID);

	glDeleteShader(vShader);
	glDeleteShader(fShader);

}

SHADER Shader7::loadFromFile(const std::string source)
{
	std::fstream f(source);

	enum shaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::stringstream ss[2];
	std::string line;
	shaderType Type = NONE;

	while (getline(f, line)) {
		if (line.find("#vertexShader") != std::string::npos) {
			Type = VERTEX;
		}
		else if (line.find("#fragmentShader") != std::string::npos) {
			Type = FRAGMENT;
		}
		else {
			ss[Type] << line << "\n";
		}
	}

	return { ss[0].str(), ss[1].str() };
}



void Shader7::SetUniform1i(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader7::SetUniform1f(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader7::SetUniform3f(const std::string& name, float x, float y, float z)
{
	glUniform3f(GetUniformLocation(name), x, y, z);
}

void Shader7::SetUniform3f(const std::string& name, glm::vec3 a)
{
	glUniform3f(GetUniformLocation(name), a.x, a.y, a.z);
}

void Shader7::SetUniform4f(const std::string& name, float x, float y, float z, float w)
{
	glUniform4f(GetUniformLocation(name), x, y, z, w);
}

void Shader7::SetUniformMat4f(const std::string& name, const glm::mat4 matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

int Shader7::GetUniformLocation(const std::string& name)
{
	if (uniformLocationSet.find(name) != uniformLocationSet.end())
		return uniformLocationSet[name];

	int location = glGetUniformLocation(ID, name.c_str());
	if (location == -1) {
		std::cout << "Warning: uniform '" << name << "' doesn't exist " << std::endl;
	}
	uniformLocationSet[name] = location;
	return location;
}

void Shader7::printVertexShader() {
	std::cout << sd.vertexShader << std::endl;
}

void Shader7::printFragmentShader() {
	std::cout << sd.fragmentShader << std::endl;
}