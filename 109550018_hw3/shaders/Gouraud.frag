#version 330 core

in vec2 texCoord;
in vec4 worldPos;
in vec3 normal;
in vec4 frag_color;
in vec4 spec;

uniform sampler2D ourTexture;

out vec4 FragColor;

void main()
{
	// get texture
	vec4 object_color = texture(ourTexture, texCoord);

	FragColor = frag_color * object_color + spec;
}