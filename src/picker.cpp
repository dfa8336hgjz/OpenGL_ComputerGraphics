#include "picker.h"
#include <iostream>
#include <SFML\Audio.hpp>


Picker7::Picker7(int width, int height) : count(0), width(width), height(height), sunRise(0), doorOpen(0), hold(0), bulb1On(0), piano(0)
{
	offsceneFB =  new FrameBuffer7(width, height);
	FBshader = new Shader7("shaders/onlyColor.shader");
	cursorAt = 100;
}

Picker7::~Picker7()
{
	if (offsceneFB) delete offsceneFB;
	if (FBshader) delete FBshader;
}

void Picker7::addObject(Object7* newObj)
{
	int r = count & 0xFF;
	int g = (count >> 8) & 0xFF;
	int b = (count >> 16) & 0xFF;

	newObj->offR = static_cast<float>(r) / 255.0f;
	newObj->offB = static_cast<float>(g) / 255.0f;
	newObj->offG = static_cast<float>(b) / 255.0f;

	count++;
	pickableObj.push_back(newObj);
}

void Picker7::renderOffScene()
{
	offsceneFB->bind();
	FBshader->bind();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height);

	for (int i = 0; i < pickableObj.size(); i++) {
		FBshader->SetUniformMat4f("model", pickableObj.at(i)->model);
		FBshader->SetUniformMat4f("projection", pickableObj.at(i)->projection);
		FBshader->SetUniformMat4f("view", pickableObj.at(i)->view);
		FBshader->SetUniform3f("Color", glm::vec3(pickableObj.at(i)->offR, pickableObj.at(i)->offG, pickableObj.at(i)->offB));
		pickableObj.at(i)->render();
	}
	FBshader->unbind();
	offsceneFB->bindDefault();
}

void Picker7::getIdAtCursor()
{
	offsceneFB->bind();
	std::vector<GLubyte> result(4, 0);
	glReadPixels(width / 2, height / 2, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, result.data());
	int r = static_cast<int>(result[0]);
	int g = static_cast<int>(result[1]);
	int b = static_cast<int>(result[2]);
	cursorAt = r | (g << 8) | (b << 16);
	//std::cout << cursorAt << std::endl;
	offsceneFB->bindDefault();
}

void Picker7::handleMouseClick(bool* keys)
{
	if (keys[GLFW_MOUSE_BUTTON_LEFT] && cursorAt == 0 && !hold) {
		sunRise = true;
		hold = true;
	}

	if (keys[GLFW_MOUSE_BUTTON_LEFT] && cursorAt == 1 && !hold) {
		hold = true;
		doorOpen = !doorOpen;
	}

	if (keys[GLFW_MOUSE_BUTTON_LEFT] && cursorAt == 2 && !hold) {
		hold = true;
		bulb1On = !bulb1On;
	}

	if (keys[GLFW_MOUSE_BUTTON_LEFT] && cursorAt == 3 && !hold) {
		hold = true;
		piano = true;
	}


	if (!keys[GLFW_MOUSE_BUTTON_LEFT] && hold) {
		hold = false;
	}
}
