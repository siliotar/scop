#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

uniform vec4 perspective;
uniform mat4 view;
uniform mat3 rotmat;

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

void main()
{
	vec3 pos = position * rotmat;
	mat4 proj = getPerspective();
	gl_Position = proj * view * vec4(pos, 1.0);
	v_texCoord = texCoord;
	float val = (gl_VertexID % 12) / 3 * 0.25f;
	v_Color = vec4(val, val, val, 1.0f);
	Normal = normal * rotmat;
	FragPos = pos;
};
