#include "texture.h"

Texture7::Texture7(const std::string& filePath, unsigned int sourceFormat) : width(0), height(0), ID(0), numOfColorChannel(0)
{
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &numOfColorChannel, 0);

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, sourceFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
}

Texture7::Texture7()
{
    path = "";
    type = "";
    width = 0;
    height = 0;
    numOfColorChannel = 0;
    ID = 0;
}

Texture7::~Texture7()
{
    glDeleteTextures(1, &ID);
}

void Texture7::bind(unsigned int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, ID);
}


void Texture7::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

inline int Texture7::getWidth() const
{
	return width;
}

inline int Texture7::getHeight() const
{
	return height;
}
