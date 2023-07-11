#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "object.h"

struct Mesh
{
	unsigned int vao, vbo1, vbo2, vbo3, ibo;

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> tex_coords;
	std::vector<unsigned int> indices;
	unsigned int tex_handle;
};

struct Texture
{
	unsigned int textureID;
	std::string image_name;
};

class Model7 : public Object7
{
private:
	Assimp::Importer importer;
	bool flip;
	const aiScene* scene = nullptr;

public:
	
	unsigned int num_meshes;
	std::vector<Mesh> mesh_list;
	std::vector<Texture> texture_list;

	Model7(const char* model_path, bool flip = 0);
	void render();

private:
	void loadModel();
	void setBufferData(unsigned int index);
	int isImageLoaded(std::string file_name);
	unsigned int loadTextureImage(std::string file_name, bool& load_complete);
};