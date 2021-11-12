#include "vector3.h"

float	lengthv3(t_vector3 vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

void	normalizev3(t_vector3* vec)
{
	float	ln = lengthv3(*vec);
	float	invLn = 1 / ln;
	vec->x *= invLn;
	vec->y *= invLn;
	vec->z *= invLn;
}

void	crossv3(t_vector3* vec, t_vector3 a, t_vector3 b)
{
	vec->x = a.y * b.z - a.z * b.y;
	vec->y = a.z * b.x - a.x * b.z;
	vec->z = a.x * b.y - a.y * b.x;
}

t_vector3* createVector3(float x, float y, float z)
{
	t_vector3* ret;
	ret = malloc(sizeof(t_vector3));
	if (ret == NULL)
		return NULL;
	ret->x = x;
	ret->y = y;
	ret->z = z;
	return ret;
}

void	multiplyvec3(t_vector3* vec, float multiplier)
{
	vec->x *= multiplier;
	vec->y *= multiplier;
	vec->z *= multiplier;
}

void	sumvec3(t_vector3* vec1, t_vector3 vec2, float multiplier)
{
	vec1->x += vec2.x * multiplier;
	vec1->y += vec2.y * multiplier;
	vec1->z += vec2.z * multiplier;
}

void	copyvec3(t_vector3* dst, t_vector3* src)
{
	dst->x = src->x;
	dst->y = src->y;
	dst->z = src->z;
}

void	setzerovec3(t_vector3* vec)
{
	vec->x = 0.0f;
	vec->y = 0.0f;
	vec->z = 0.0f;
}

float	dotvec3(t_vector3 vec1, t_vector3 vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}
