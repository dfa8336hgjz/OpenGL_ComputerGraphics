#vertexShader
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
	TexCoords = aTexCoord;
	Normal = transpose(inverse(mat3(model))) * aNormal;
	if (length(Normal) > 0)
		Normal = normalize(Normal);
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#fragmentShader
#version 330 core
out vec4 FragColor;

struct DirectionalLight{
	vec3 position;
	float ambient;
	float diffuse;
	float specular;
};

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	float ambient;
	float diffuse;
	float specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 camPos;
uniform vec3 lightPos;
uniform DirectionalLight dirLight;
uniform PointLight pointLight;
uniform sampler2D txt;
uniform float material;

void main()
{
	vec3 viewDirection = normalize(camPos - FragPos);
	vec3 lightDirection = normalize(vec3(dirLight.position - FragPos));

	vec4 mTexture = texture(txt, TexCoords);

	vec4 ambient = vec4(dirLight.ambient * mTexture.rgb, 1.0);

	vec4 diffuse = vec4(0.0);
	vec4 specular = vec4(0.0);

	if (length(dirLight.position) > 0) {
		float diffuseAngle = max(dot(lightDirection, Normal), 0.0);
		diffuse = vec4(dirLight.diffuse * diffuseAngle * mTexture.rgb, 1.0);

		vec3 reflectDirection = normalize(reflect(-lightDirection, Normal));
		float specStrength = pow(max(dot(viewDirection, reflectDirection), 0), material);
		specular = vec4(vec3(dirLight.specular) * specStrength, 1.0);
	}

	vec4 dirlight = ambient + diffuse + specular;


	vec4 pointlight = vec4(0.0);
	
	if (length(pointLight.position) > 0) {
		ambient = vec4(pointLight.ambient * vec3(mTexture), 1.0);

		vec3 lightDir = normalize(pointLight.position - FragPos);
		float diff = max(dot(Normal, lightDir), 0.0);
		diffuse = vec4(pointLight.diffuse * diff * vec3(mTexture), 1.0);

		vec3 reflectDir = reflect(-lightDir, Normal);
		float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material);
		float distance = length(pointLight.position - FragPos);
		float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));
		specular = vec4(pointLight.specular * spec * vec3(mTexture), 1.0);

		pointlight += ambient + diffuse + specular;
	}
	

	FragColor = dirlight + pointlight;
}