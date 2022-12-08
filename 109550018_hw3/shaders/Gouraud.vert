#version 330 core

// TODO:
// Implement Gouraud shading

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float gloss;
};

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
};

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform Material material;
uniform Light light;
uniform vec3 camera_position;

out vec2 texCoord;
out vec4 worldPos;
out vec3 normal;
out vec4 frag_color;
out vec4 spec;

void main()
{
	gl_Position = P * V * M * vec4(aPos, 1.0);
	texCoord = aTexCoord;
	worldPos = M * vec4(aPos, 1.0);
	mat4 normal_transform = transpose(inverse(M));
	normal = normalize((normal_transform * vec4(aNormal, 0.0)).xyz);

	// parameters of model material
	vec4 Ka = vec4(material.ambient, 1.0f), 
		 Kd = vec4(material.diffuse, 1.0f), 
		 Ks = vec4(material.specular, 1.0f);
	float gloss = material.gloss;

	// parameters of loght
	vec4 La = vec4(light.ambient, 1.0f), 
		 Ld = vec4(light.diffuse, 1.0f),
		 Ls = vec4(light.specular, 1.0f);
	vec3 lightPos = light.position, 
		 n_normal = normalize(normal), 
		 light = (normalize(vec4(lightPos, 1.0) - worldPos)).xyz;

	// parameters of view and reflect
	vec3 view = normalize(camera_position - worldPos.xyz), // ???
		 reflect = normalize(reflect(-light, normal));

	// ambient, diffuse, specular
	vec4 ambient = La * Ka, 
		 diffuse = Ld * Kd * max(dot(light, normal), 0.0), 
		 specular = Ls * Ks * pow(max(dot(view, reflect), 0.0), gloss);

	frag_color = (ambient + diffuse);
	spec = specular;
}