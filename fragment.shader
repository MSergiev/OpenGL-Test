#version 330 core

out vec4 color;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Light{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material{
	vec3 ambient;
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};

uniform Light light;
uniform Material material;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos;


void main(){
	vec3 ambient = light.ambient*vec3(texture(material.diffuse, TexCoords));
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos-FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse*diff*vec3(texture(material.diffuse, TexCoords));
	float specularStrength = 0.8f;
	vec3 viewDir = normalize(viewPos-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), 32);
	vec3 specular = light.specular*(spec*material.specular);
	vec3 result = ambient+diffuse+specular;
	color = vec4(result, 1.0f);
}
