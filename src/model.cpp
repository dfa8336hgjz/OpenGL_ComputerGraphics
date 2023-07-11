#define STB_IMAGE_IMPLEMENTATION
#include "model.h"
#include "stb_image.h"


Model7::Model7(const char* model_path, bool flip)
{
	this->flip = flip;
	scene = importer.ReadFile(model_path, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipUVs);
	loadModel();
}

void Model7::loadModel()
{
	if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
		std::cout << "Assimp importer.ReadFile (Error) -- " << importer.GetErrorString() << "\n";
	else
	{
		num_meshes = scene->mNumMeshes;
		mesh_list.resize(num_meshes);

		aiMesh* mesh;
		int indices_offset = 0; 

		for (unsigned int i = 0; i < num_meshes; ++i)
		{
			mesh = scene->mMeshes[i];

			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			for (unsigned int tex_count = 0; tex_count < material->GetTextureCount(aiTextureType_DIFFUSE); ++tex_count)
			{
				aiString string;
				material->GetTexture(aiTextureType_DIFFUSE, tex_count, &string); 

				int already_loaded = isImageLoaded(string.C_Str()); 

				if (already_loaded == -1)
				{
					bool load_success = false;
					unsigned int texture_handle = loadTextureImage(string.C_Str(), load_success);

					if (load_success)
					{
						Texture texture;
						texture.image_name = string.C_Str();
						texture.textureID = texture_handle;

						texture_list.push_back(texture);
						mesh_list[i].tex_handle = texture_handle;
					}
				}
				else
					mesh_list[i].tex_handle = already_loaded;
			}

			for (unsigned int i2 = 0; i2 < mesh->mNumVertices; ++i2)
			{
				glm::vec3 pos;
				pos.x = mesh->mVertices[i2].x;
				pos.y = mesh->mVertices[i2].y;
				pos.z = mesh->mVertices[i2].z;
				mesh_list[i].positions.push_back(pos);

				if (mesh->HasNormals())
				{
					glm::vec3 normal;
					normal.x = mesh->mNormals[i2].x;
					normal.y = mesh->mNormals[i2].y;
					normal.z = mesh->mNormals[i2].z;
					mesh_list[i].normals.push_back(normal);
				}
				else
					mesh_list[i].normals.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

				if (mesh->HasTextureCoords(0))
				{
					glm::vec2 tex_coords;
					tex_coords.x = mesh->mTextureCoords[0][i2].x;
					tex_coords.y = mesh->mTextureCoords[0][i2].y;
					mesh_list[i].tex_coords.push_back(tex_coords);
				}
				else
					mesh_list[i].tex_coords.push_back(glm::vec2(0.0f, 0.0f));
			}

			for (unsigned int i3 = 0; i3 < mesh->mNumFaces; ++i3)
				for (unsigned int i4 = 0; i4 < mesh->mFaces[i3].mNumIndices; ++i4)
					mesh_list[i].indices.push_back(mesh->mFaces[i3].mIndices[i4] + indices_offset);

			setBufferData(i);
		}
	}
}

void Model7::setBufferData(unsigned int index)
{
	glGenVertexArrays(1, &mesh_list[index].vao);
	glGenBuffers(1, &mesh_list[index].vbo1);
	glGenBuffers(1, &mesh_list[index].vbo2);
	glGenBuffers(1, &mesh_list[index].vbo3);
	glGenBuffers(1, &mesh_list[index].ibo);

	glBindVertexArray(mesh_list[index].vao);


	glBindBuffer(GL_ARRAY_BUFFER, mesh_list[index].vbo1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh_list[index].positions.size(), &mesh_list[index].positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, mesh_list[index].vbo2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh_list[index].normals.size(), &mesh_list[index].normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, mesh_list[index].vbo3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh_list[index].tex_coords.size(), &mesh_list[index].tex_coords[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_list[index].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh_list[index].indices.size(), &mesh_list[index].indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

int Model7::isImageLoaded(std::string file_name)
{
	for (unsigned int i = 0; i < texture_list.size(); ++i)
		if (file_name.compare(texture_list[i].image_name) == 0)
			return texture_list[i].textureID;
	return -1;
}

unsigned int Model7::loadTextureImage(std::string file_name, bool& load_complete)
{
	stbi_set_flip_vertically_on_load(flip);

	std::size_t position = file_name.find_last_of("\\");
	file_name = "images\\" + file_name.substr(position + 1);

	int width, height, num_components;
	unsigned char* image_data = stbi_load(file_name.c_str(), &width, &height, &num_components, 0);

	unsigned int textureID;
	glGenTextures(1, &textureID);

	if (image_data)
	{
		GLenum format{};

		if (num_components == 1)
			format = GL_RED;
		else if (num_components == 3)
			format = GL_RGB;
		else if (num_components == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image_data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		load_complete = true;
		stbi_image_free(image_data);
	}
	else
	{
		load_complete = false;
		std::cout << "load fail " << file_name << std::endl;
		stbi_image_free(image_data);
	}
	return textureID;
}

void Model7::render()
{
	for (unsigned int i = 0; i < num_meshes; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, mesh_list[i].tex_handle); 

		glBindVertexArray(mesh_list[i].vao);
		glDrawElements(GL_TRIANGLES, (GLsizei)mesh_list[i].indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}
