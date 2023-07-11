#pragma once
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera7 {
public:
	glm::vec3 cameraPos, cameraFront, cameraUp;
	float charHeight;

	float cameraSpeed;
	float mouseSensitivity;
	float zoom;

	float yaw;
	float pitch;
	glm::mat4 view;

	Camera7();
	~Camera7();

	glm::mat4 getViewMatrix();

};
