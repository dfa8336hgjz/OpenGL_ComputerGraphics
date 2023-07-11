#include "camera.h"

Camera7::Camera7(): pitch(0.0f), yaw(-90.0f), zoom(45.0f), mouseSensitivity(0.099f)
    , cameraSpeed(20.0f), charHeight(9.0f)
{
    cameraPos = glm::vec3(0.0f, charHeight, 10.0f);
    cameraFront = glm::vec3(0.0f, charHeight, -3.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

Camera7::~Camera7()
{
}

glm::mat4 Camera7::getViewMatrix()
{
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));
    return view;
}