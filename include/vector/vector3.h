#ifndef VECTOR3_H
# define VECTOR3_H

# include <stdlib.h>
# include <math.h>

typedef struct	s_vector3
{
	float	x;
	float	y;
	float	z;
}				t_vector3;

t_vector3	*createVector3(float x, float y, float z);
void		multiplyvec3(t_vector3* vec, float multiplier);
void		copyvec3(t_vector3* dst, t_vector3* src);
void		setzerovec3(t_vector3* vec);
void		setVector3(t_vector3* ret, float x, float y, float z);

float		lengthv3(t_vector3 vec);
void		normalizev3(t_vector3* vec);
void		crossv3(t_vector3* vec, t_vector3 a, t_vector3 b);
void		sumvec3(t_vector3* vec1, t_vector3 vec2, float multiplier);
float		dotvec3(t_vector3 vec1, t_vector3 vec2);

#endif
