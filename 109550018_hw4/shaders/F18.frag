#version 330 core

in vec2 texCoord;
in vec3 normal;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

uniform int ball;
uniform int colorControl;

out vec4 FragColor;

void main()
{
	FragColor = mix(texture(ourTexture, texCoord), texture(ourTexture1, texCoord), 0.5);
}