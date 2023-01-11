#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 normalM;
uniform int colorControl;

out VS_OUT {
	vec3 normal;
	vec2 texCoord;
	vec4 color;
} vs_out;

void main()
{
	gl_Position = P * V * M * vec4(aPos, 1.0);
	vs_out.texCoord = aTexCoord;
	vs_out.normal = normalize((normalM * vec4(aNormal, 0.0)).xyz);
	if (colorControl == 3) {
		vs_out.color = vec4(0.75, 0.75, 0.2, 0.0);
	} else if (colorControl == 2) {
		vs_out.color = vec4(0.9, 0.6, 0.4, 0.0);
	} else {
		vs_out.color = vec4(0.8, 0.2, 0.2, 0.0);
	}
}