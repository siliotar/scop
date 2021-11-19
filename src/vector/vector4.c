#include "vector/vector4.h"

void	setVector4(t_vector4* ret, float x, float y, float z, float w)
{
	ret->x = x;
	ret->y = y;
	ret->z = z;
	ret->w = w;
}

void	multiplyvec4(t_vector4* vec, float multiplier)
{
	vec->x *= multiplier;
	vec->y *= multiplier;
	vec->z *= multiplier;
	vec->w *= multiplier;
}

void	sumvec4(t_vector4* vec1, t_vector4* vec2)
{
	vec1->x += vec2->x;
	vec1->y += vec2->y;
	vec1->z += vec2->z;
	vec1->w += vec2->w;
}

void	copyvec4(t_vector4* dst, t_vector4* src)
{
	dst->x = src->x;
	dst->y = src->y;
	dst->z = src->z;
	dst->w = src->w;
}

void	setzerovec4(t_vector4* vec)
{
	vec->x = 0.0f;
	vec->y = 0.0f;
	vec->z = 0.0f;
	vec->w = 0.0f;
}
