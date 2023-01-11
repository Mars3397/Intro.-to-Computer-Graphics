#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
	vec3 normal;
	vec2 texCoord;
	vec4 color;
} gs_in[];

uniform float rand_float;
uniform float rand_float1;

out vec3 normal;
out vec2 texCoord;
out vec4 color;

void main()
{
	for (int i = 0; i < 3; i++)
	{
		gl_Position = gl_in[i].gl_Position;
		normal = gs_in[i].normal;
		texCoord = gs_in[i].texCoord;
		color = gs_in[i].color;
		EmitVertex();
	}
	gl_Position = gl_in[0].gl_Position + vec4(gs_in[0].normal, 0.0) * rand_float1 * 2;
	normal = gs_in[0].normal;
	texCoord = gs_in[0].texCoord;
	color = vec4(0.5, 0.5, 0.5, 0.0);
	EmitVertex();
	EndPrimitive();
}