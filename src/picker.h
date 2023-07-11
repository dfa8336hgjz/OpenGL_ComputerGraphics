#pragma once
#include <vector>
#include "object.h"
#include "frameBuffer.h"
#include "shaders.h"

class Picker7
{
	Shader7* FBshader;
	int count;
	int cursorAt;
	int width, height;
	std::vector<Object7*> pickableObj;

	bool hold;

public:
	bool sunRise;
	bool doorOpen;
	bool bulb1On;
	bool piano;

	FrameBuffer7* offsceneFB;
	Picker7(int width, int height);
	~Picker7();
	
	void addObject(Object7* newObj);
	void renderOffScene();
	void getIdAtCursor();
	void handleMouseClick(bool* keys);
};