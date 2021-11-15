#ifndef MATRIX_H
# define MATRIX_H

# include <stdlib.h>
# include <math.h>
# include "../vector/vector4.h"

typedef struct	s_matrix4
{
	t_vector4	field[4];
}				t_matrix4;

void	createMatrix4(t_matrix4* mat, float value);
void	deleteMatrix4(t_matrix4* mat);
void	translatemat4(t_matrix4* mat, float x, float y, float z);
void	perspectivemat4(t_matrix4* mat, float fovy, float aspect, float zNear, float zFar);
float	*value_ptrmat4(t_matrix4* mat);

#endif
