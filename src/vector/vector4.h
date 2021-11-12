#ifndef VECTOR4_H
# define VECTOR4_H

# include <stdlib.h>

typedef struct	s_vector4
{
	float	x;
	float	y;
	float	z;
	float	w;
}				t_vector4;

void		setVector4(t_vector4* ret, float x, float y, float z, float w);
void		multiplyvec4(t_vector4* vec, float multiplier);
void		sumvec4(t_vector4* vec1, t_vector4* vec2);
void		copyvec4(t_vector4* dst, t_vector4* src);
void		setzerovec4(t_vector4* vec);

#endif
