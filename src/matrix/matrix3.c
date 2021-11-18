#include "matrix3.h"

void	createMatrix3(t_matrix3 *mat, float value)
{
	setVector3(&(mat->field[0]), value, 0.0f, 0.0f);
	setVector3(&(mat->field[1]), 0.0f, value, 0.0f);
	setVector3(&(mat->field[2]), 0.0f, 0.0f, value);
}

t_matrix3	getxrot(float alpha)
{
	t_matrix3	mat;

	setVector3(&(mat.field[0]), 1.0f, 0.0f, 0.0f);
	setVector3(&(mat.field[1]), 0.0f, cos(alpha), -sin(alpha));
	setVector3(&(mat.field[2]), 0.0f, sin(alpha), cos(alpha));
	return mat;
}

t_matrix3	getyrot(float alpha)
{
	t_matrix3	mat;

	setVector3(&(mat.field[0]), cos(alpha), 0.0, sin(alpha));
	setVector3(&(mat.field[1]), 0.0, 1.0, 0.0);
	setVector3(&(mat.field[2]), -sin(alpha), 0.0, cos(alpha));
	return mat;
}

t_matrix3	getzrot(float alpha)
{
	t_matrix3	mat;

	setVector3(&(mat.field[0]), cos(alpha), -sin(alpha), 0.0);
	setVector3(&(mat.field[1]), sin(alpha), cos(alpha), 0.0);
	setVector3(&(mat.field[2]), 0.0, 0.0, 1.0);
	return mat;
}

t_matrix3	muliplyMat3(t_matrix3 m1, t_matrix3 m2)
{
	t_matrix3	mat;

	for (int i = 0; i < 3; ++i)
	{
		float	value1 = m1.field[i].x * m2.field[0].x + m1.field[i].y * m2.field[1].x + m1.field[i].z * m2.field[2].x;
		float	value2 = m1.field[i].x * m2.field[0].y + m1.field[i].y * m2.field[1].y + m1.field[i].z * m2.field[2].y;
		float	value3 = m1.field[i].x * m2.field[0].z + m1.field[i].y * m2.field[1].z + m1.field[i].z * m2.field[2].z;
		setVector3(&(mat.field[i]), value1, value2, value3);
	}
	return mat;
}
