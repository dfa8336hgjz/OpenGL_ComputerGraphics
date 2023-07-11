#pragma once
#include <GL/glew.h>
#include "stb_image.h"
#include <string>
#include <iostream>

class Texture7 {
	int width, height, numOfColorChannel;
	unsigned int ID;

public:
	std::string path;
	std::string type;
	Texture7(const std::string& filePath, unsigned int sourceFormat = GL_RGB);
	Texture7();
	~Texture7();

	void bind(unsigned int slot);
	void unbind();

	inline int getWidth() const;
	inline int getHeight() const;

};