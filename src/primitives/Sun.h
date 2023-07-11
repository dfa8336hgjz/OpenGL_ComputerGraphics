#pragma once
#include "model.h"
#include "shaders.h"

class Sun
{
	float increment;
	glm::mat4 model, view, projection;
	glm::vec3 pos;

	Shader7* sd;
	Model7* sun;
	
public:
	float riseY;
	Sun(const char* model_path);
	~Sun();

	void render();
	glm::vec3 getPos();
	void getViewProjection(glm::mat4 view, glm::mat4 projection);
	void rise();
};