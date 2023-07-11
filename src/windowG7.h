#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include <SFML\Audio.hpp>
#include "camera.h"

static int re_W = 0;
static int re_H = 0;

class Window7
{
	bool objectPicked;

protected:
	int width, height;
	float cursorX, cursorY;
	float deltaTime;
	float lastFrame;
	bool firstMouse;
	Camera7 cam;
	bool hold;

	static void mousePosCallback(GLFWwindow* window, double xposIn, double yposIn);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
	float camZoom;
	GLFWwindow* win;
	bool fbMode;
	bool keys[1024];

	Window7(int width, int height, const std::string str, GLFWmonitor* monitor, GLFWwindow* sharedWindow);
	~Window7();

	bool windowCreated();
	bool keyPressedOnce(int key);
	bool canClose();

	int getWidth();
	int getHeight();

	inline void setSize(int n_width, int n_height);
	void setCallback();
	void swapBuffer();
	glm::mat4 getViewMatrix();
	glm::vec3 getViewPos();
	glm::vec3 getViewFront();
	void keyHandle();

	void getDeltaTime();
};