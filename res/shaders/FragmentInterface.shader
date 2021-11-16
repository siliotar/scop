#version 450 core

in vec2 TexCoords;

uniform sampler2D tex;

out vec4 color;

void main()
{
	vec4 temp = texture(tex, TexCoords);
	if (temp.x < 0.5f)
		discard;
	color = temp;
};
