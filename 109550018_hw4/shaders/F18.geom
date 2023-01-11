#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
	vec3 normal;
	vec2 texCoord;
} gs_in[];

out vec3 normal;
out vec2 texCoord;

void main()
{
	for (int i = 0; i < 3; i++)
	{
		gl_Position = gl_in[i].gl_Position;
		normal = gs_in[i].normal;
		texCoord = gs_in[i].texCoord;
		EmitVertex();
	}
	EndPrimitive();
}