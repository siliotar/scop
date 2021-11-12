#ifndef SCOP_H
# define SCOP_H


typedef struct s_texture
{
	int	bpp;
	int	width;
	int	height;
	unsigned char	*data;
}t_texture;

typedef struct s_scop
{
	float	anglex;
	float	angley;
	float	anglez;
	char	texSign;
	unsigned char	tempTex;
	float	texTrans;
	t_texture	defaultTexture;
	t_texture	customTexture;
}t_scop;

void	initScop(t_scop *scop);

#endif