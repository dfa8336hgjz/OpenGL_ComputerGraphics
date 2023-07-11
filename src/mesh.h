#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "object.h"

#include <vector>
#include <string>

class Mesh7 : public Object7
{
	unsigned int vao, vbo, ibo;
public:
	int indexCount;

	Mesh7();
	~Mesh7();

	void createMesh(unsigned int drawMode, const std::vector<float> vertices, 
		unsigned int numOfVertices, const std::vector<unsigned int> indices, unsigned int numOfIndices);
	void clearMesh();
	void render();

	void bind();
	void unbind();

};