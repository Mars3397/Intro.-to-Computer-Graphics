#version 330 core

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

in vec2 texCoord;
in vec4 worldPos;
in vec3 normal;

uniform sampler2D ourTexture;
uniform Material material;
uniform Light light;
uniform vec3 camera_position;

out vec4 FragColor;

void main()
{
	vec4 object_color = texture(ourTexture, texCoord);

	// parameters of model material
	vec4 Ka = vec4(material.ambient, 1.0f), 
		 Kd = vec4(material.diffuse, 1.0f), 
		 Ks = vec4(material.specular, 1.0f);
	float gloss = material.gloss;

	// parameters of light
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
	vec4 ambient = La * Ka * object_color, 
		 diffuse = Ld * Kd * object_color * max(dot(light, normal), 0.0), 
		 specular = Ls * Ks * pow(max(dot(view, reflect), 0.0), gloss);

	FragColor = (ambient + diffuse + specular);
}