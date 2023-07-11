#vertexShader
#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#fragmentShader
#version 330 core
out vec4 FragColor;

uniform vec3 Color;

void main()
{
	FragColor = vec4(Color, 1.0f);
}