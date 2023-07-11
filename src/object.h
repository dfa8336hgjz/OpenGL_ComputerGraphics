#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

class Object7
{
public:
	Object7(){
		offR = offG = offB = 0.0f;
	};

	~Object7(){};

	float offR, offG, offB;
	glm::mat4 model;
	glm::mat4 projection;
	glm::mat4 view;
	
	void getMVP(glm::mat4 model, glm::mat4 projection, glm::mat4 view) {
		this->model = model;
		this->projection = projection;
		this->view = view;
	};

	virtual void render() = 0;
};