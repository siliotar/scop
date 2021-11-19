#version 410 core

in vec2 TexCoords;

uniform sampler2D tex;
uniform vec4 u_Color;
uniform int u_SimpleColor;

out vec4 color;

void main()
{
	vec4 temp = texture(tex, TexCoords);
	if (u_SimpleColor == 1)
	{
		if (u_Color.w < 0.5f)
			temp = vec4(vec3(u_Color), temp.w);
		else
			temp = u_Color;
	}
	if (temp.w < 0.5f)
		discard;
	color = temp;
}
