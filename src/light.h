#pragma once
#pragma once
#include "shaders.h"

class Light7 {
	glm::vec3 bulbPos = glm::vec3(0.0f, 25.0f, 50.0f);

public:
	Light7(Shader7* shader);
	~Light7();

	void setCam(Shader7* shader, glm::vec3 position);
	void useSunLight(Shader7* shader, glm::vec3 position);
	void useBulb1(Shader7* shader);
	void noBulb1(Shader7* shader);
	void setMaterial(Shader7* shader, float material);
};