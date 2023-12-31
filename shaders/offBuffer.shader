#vertexShader
#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoords;

out vec2 texCoords;

void main()
{
    gl_Position = vec4(inPos, 1.0); 
    texCoords = inTexCoords;
}  

#fragmentShader
#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;


void main()
{
    FragColor = texture(screenTexture, texCoords);
}