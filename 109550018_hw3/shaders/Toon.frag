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

// threshold
float high_intensity = 1.0f, 
	  medium_intensity = 0.65f,
	  low_intensity = 0.1f;

void main()
{
	vec4 object_color = texture(ourTexture, texCoord);

	// parameters of model material
	vec4 Kd = vec4(material.diffuse, 1.0f), 
		 Ks = vec4(material.specular, 1.0f);
	float gloss = material.gloss;

	// parameters of loght
	vec4 Ls = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	vec3 lightPos = light.position, 
		 n_normal = normalize(normal), 
		 light = (normalize(vec4(lightPos, 1.0) - worldPos)).xyz;

	// parameters of view and reflect
	vec3 view = normalize(camera_position - worldPos.xyz),
		 reflect = normalize(reflect(-light, normal));

	// ambient, diffuse, specular
	vec4 specular = Ls * Ks * pow(max(dot(view, reflect), 0.0), gloss);

	float intensity;
	if (dot(light, normal) < 0) {
		intensity = low_intensity;
	} else if (specular.x > 0.01 || specular.y > 0.01 || specular.z > 0.01) {
		intensity = high_intensity;
	} else {
		intensity = medium_intensity;
	}

	FragColor = Kd * object_color * intensity;
}