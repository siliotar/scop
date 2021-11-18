#ifndef MATRIX3_H
# define MATRIX3_H

# include <stdlib.h>
# include <math.h>
# include "../vector/vector3.h"

typedef struct	s_matrix3
{
	t_vector3	field[3];
}				t_matrix3;

void    	createMatrix3(t_matrix3* mat, float value);
t_matrix3	getxrot(float alpha);
t_matrix3	getyrot(float alpha);
t_matrix3	getzrot(float alpha);
t_matrix3	muliplyMat3(t_matrix3 m1, t_matrix3 m2);

#endif
