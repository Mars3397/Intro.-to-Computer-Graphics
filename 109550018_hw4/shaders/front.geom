#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
	vec3 normal;
	vec2 texCoord;
	vec4 position;
} gs_in[];

uniform mat4 P;
uniform float rand_float;

out vec3 color;

const float MAGNITUDE = 0.05;

void GenerateLine(int index)
{
	color = vec3(1, 1, 1);
	gl_Position = P * gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = P * (gl_in[index].gl_Position + vec4(0.0, -3, 6, 0.0) * rand_float);
	EmitVertex();
	EndPrimitive();
}

void main()
{
	if (gs_in[0].texCoord.x > 0.125 && gs_in[0].texCoord.x < 0.175 &&
		(gs_in[0].texCoord.y > 0.64 && gs_in[0].texCoord.y < 0.675) &&
		dot(vec3(0, 0, -1), gs_in[0].normal) > -0.2) {
		GenerateLine(0);
	}
	if (gs_in[1].texCoord.x > 0.125 && gs_in[1].texCoord.x < 0.175 &&
		(gs_in[1].texCoord.y > 0.64 && gs_in[1].texCoord.y < 0.675) &&
		dot(vec3(0, 0, -1), gs_in[1].normal) > -0.2) {
		GenerateLine(1);
	}
	if (gs_in[2].texCoord.x > 0.125 && gs_in[2].texCoord.x < 0.175 &&
		(gs_in[2].texCoord.y > 0.64 && gs_in[2].texCoord.y < 0.675) &&
		dot(vec3(0, 0, -1), gs_in[2].normal) > -0.2) {
		GenerateLine(2);
	}


}