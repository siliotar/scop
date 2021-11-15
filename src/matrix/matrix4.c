#include "matrix4.h"

void	createMatrix4(t_matrix4 *mat, float value)
{
	setVector4(&(mat->field[0]), value, 0.0f, 0.0f, 0.0f);
	setVector4(&(mat->field[1]), 0.0f, value, 0.0f, 0.0f);
	setVector4(&(mat->field[2]), 0.0f, 0.0f, value, 0.0f);
	setVector4(&(mat->field[3]), 0.0f, 0.0f, 0.0f, value);
}

void	deleteMatrix4(t_matrix4* mat)
{
	free(mat);
}

void	translatemat4(t_matrix4 *mat, float x, float y, float z)
{
	t_vector4	tmp;
	t_vector4	tmp2;

	copyvec4(&tmp, &(mat->field[0]));
	multiplyvec4(&tmp, x);
	copyvec4(&tmp2, &(mat->field[1]));
	multiplyvec4(&tmp2, y);
	sumvec4(&tmp, &tmp2);
	copyvec4(&tmp2, &(mat->field[2]));
	multiplyvec4(&tmp2, z);
	sumvec4(&tmp, &tmp2);
	sumvec4(&tmp, &(mat->field[3]));
	mat->field[3] = tmp;
}

void	setzeromat4(t_matrix4* mat)
{
	int i;

	i = -1;
	while (++i < 4)
		setzerovec4(&((mat->field)[i]));
}

void	perspectivemat4(t_matrix4* mat, float fovy, float aspect, float zNear, float zFar)
{
	float	tanHalfFovy;

	tanHalfFovy = tan(fovy / 2.0f);
	setzeromat4(mat);
	mat->field[0].x = 1.0f / (aspect * tanHalfFovy);
	mat->field[1].y = 1.0f / tanHalfFovy;
	mat->field[2].z = - (zFar + zNear) / (zFar - zNear);
	mat->field[2].w = -1.0f;
	mat->field[3].z = -(2.0f * zFar * zNear) / (zFar - zNear);
}

float	*value_ptrmat4(t_matrix4 *mat)
{
	return &(mat->field[0].x);
}
