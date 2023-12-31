#vertexShader
#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}

#fragmentShader
#version 330 core
out vec4 FragColor;
in vec3 TexCoords;

uniform float brightness;
uniform samplerCube skybox;

void main()
{
    FragColor = brightness * texture(skybox, TexCoords);
}