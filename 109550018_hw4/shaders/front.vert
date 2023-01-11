#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 M;
uniform mat4 normalM;
uniform mat4 V;

out VS_OUT {
	vec3 normal;
	vec2 texCoord;
	vec4 position;
} vs_out;

vec4 worldPos;

void main()
{
	worldPos = M * vec4(aPos, 1.0);
	gl_Position = V * worldPos;
	vs_out.position = worldPos;
	vs_out.texCoord = aTexCoord;
	vs_out.normal = normalize((normalM * vec4(aNormal, 0.0)).xyz);;
}