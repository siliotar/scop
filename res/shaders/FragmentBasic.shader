#version 450 core

out vec4 FragColor;

in vec4 v_Color;
in vec2 v_texCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D u_Texture;
uniform float u_texTrans;
uniform vec4 u_Color;
uniform int u_SimpleColor;
uniform vec3 u_lightColor;
uniform vec3 u_lightPos;

void main()
{
	float	ambientStrength = 0.1f;
	vec3	ambient = ambientStrength * u_lightColor;
	vec3	normal = normalize(Normal);
	vec3	lightDir = normalize(u_lightPos - FragPos);
	float	diff = max(dot(normal, lightDir), 0.0f);
	vec3	diffuse = diff * u_lightColor;
	vec4	objectColor;
	if (u_SimpleColor == 0)
		objectColor = max(u_Color * u_texTrans, v_Color * (1.0f - u_texTrans));
	else
		objectColor = max(texture(u_Texture, v_texCoord) * u_texTrans, v_Color * (1.0f - u_texTrans));
	vec3	result = (ambient + diffuse) * vec3(objectColor);
	FragColor = vec4(result, 1.0f);
};
