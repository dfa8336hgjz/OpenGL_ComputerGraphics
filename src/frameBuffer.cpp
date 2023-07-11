#include "frameBuffer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

FrameBuffer7::FrameBuffer7(int width, int height)
{
	this->width = width;
	this->height = height;
	init();
}

FrameBuffer7::~FrameBuffer7()
{
	glDeleteTextures(1, &colorTXT);
	glDeleteRenderbuffers(1, &depthRB);
	glDeleteFramebuffers(1, &ID);
}

void FrameBuffer7::init()
{
	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	glGenTextures(1, &colorTXT);
	glBindTexture(GL_TEXTURE_2D, colorTXT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &depthRB);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRB);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTXT, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRB);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer7::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void FrameBuffer7::bindDefault()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
