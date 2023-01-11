#version 330 core

in vec2 texCoord;
in vec4 worldPos;
in vec3 normal;

uniform sampler2D ourTexture;
uniform vec3 camera_position;

out vec4 FragColor;

void main()
{
	vec3 view = normalize(camera_position - worldPos.xyz);
	if (dot(view, normal) < 0.15 && dot(view, normal) > -0.15) {
		FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	} else if (dot(view, normal) < 0.2 && dot(view, normal) > -0.2) {
		FragColor = vec4(0.0f, 0.0f, 0.7f, 1.0f);
	} else if (dot(view, normal) < 0.25 && dot(view, normal) > -0.25) {
		FragColor = vec4(0.0f, 0.0f, 0.5f, 1.0f);
	} else if (dot(view, normal) < 0.3 && dot(view, normal) > -0.3) {
		FragColor = vec4(0.0f, 0.0f, 0.2f, 1.0f);
	} else {
		FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
}