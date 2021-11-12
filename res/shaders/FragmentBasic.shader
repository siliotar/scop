#version 450 core

out vec4 FragColor;

in vec4 v_Color;
in vec2 v_texCoord;
uniform sampler2D u_Texture;
uniform float u_texTrans;

void main()
{
	FragColor = max(texture(u_Texture, v_texCoord) * u_texTrans, v_Color * (1.0f - u_texTrans));
};
