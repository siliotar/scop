#version 330 core

layout(location = 3) in vec4 vertex;
out vec2 TexCoords;

void main()
{
	gl_Position = vec4(vertex.xy, -1.0f, 1.0f);
	TexCoords = vertex.zw;
};
