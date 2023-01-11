#version 330 core

in vec2 texCoord;
in vec3 normal;
in vec4 color;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

uniform int ball;
uniform int colorControl;

out vec4 FragColor;

void main()
{
	FragColor = color;
}