#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

uniform vec3 rotation;
uniform vec4 perspective;
uniform mat4 view;

out vec2 v_texCoord;
out vec4 v_Color;
out vec3 FragPos;
out vec3 Normal;

mat4 getPerspective()
{
	float tanHalfFov = tan(perspective.x / 2.0f);
	float t1 = 1.0 / (perspective.y * tanHalfFov);
	float t2 = 1.0 / tanHalfFov;
	float t3 = -(perspective.w + perspective.z) / (perspective.w - perspective.z);
	float t4 = -(2.0f * perspective.w * perspective.z) / (perspective.w - perspective.z);
	return mat4(
		t1,				0.0,			0.0,			0.0,
		0.0,			t2,				0.0,			0.0,
		0.0,			0.0,			t3,				-1.0,
		0.0,			0.0,			t4,				0.0
	);
}

mat3 getxrot(float alpha)
{
	return mat3(
		1.0,			0.0,			0.0,
		0.0,			cos(alpha),		-sin(alpha),
		0.0,			sin(alpha),		cos(alpha)
	);
}

mat3 getyrot(float alpha)
{
	return mat3(
		cos(alpha),		0.0,			sin(alpha),
		0.0,			1.0,			0.0,
		-sin(alpha),	0.0,			cos(alpha)
	);
}

mat3 getzrot(float alpha)
{
	return mat3(
		cos(alpha),		-sin(alpha),	0.0,
		sin(alpha),		cos(alpha),		0.0,
		0.0,			0.0,			1.0
	);
}

void main()
{
	mat3 rotmat = getxrot(rotation.x) * getyrot(rotation.y) * getzrot(rotation.z);
	mat4 proj = getPerspective();
	gl_Position = proj * view * vec4(position * rotmat, 1.0);
	v_texCoord = texCoord;
	float val = (gl_VertexID % 12) / 3 * 0.25f;
	v_Color = vec4(val, val, val, 1.0f);
	Normal = normal * rotmat;
	FragPos = position * rotmat;
};
