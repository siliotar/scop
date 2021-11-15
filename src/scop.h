#ifndef SCOP_H
# define SCOP_H

# include "camera.h"
# include "renderer.h"
# include "vector/vector3.h"

typedef struct s_texture
{
	int	bpp;
	int	width;
	int	height;
	unsigned char	*data;
}t_texture;

typedef struct s_locations
{
	GLint			color;
	GLint			tempTexture;
	GLint			rotation;
	GLint			lightPos;
	GLint			lightColor;
	GLint			perspective;
	GLint			view;
	GLint			texTrans;
}t_locations;

typedef struct s_scop
{
	t_vector3		rotation;
	char			texSign;
	unsigned char	tempTex;
	float			texTrans;
	t_texture		textures[5];
	unsigned char	texCount;
	t_vector3		textureColor;
	GLuint			texids[5];
	GLFWwindow		*window;
	GLuint			vao;
	GLuint			vbo;
	GLuint			ibo;
	GLuint			shader;
	t_locations		locations;
	float			deltaTime;
	t_camera		camera;
	t_matrix4		lookAt;
}t_scop;

void	initScop(t_scop *scop);
void	setLocations(t_scop *scop);
void	calculateDeltaTime(t_scop* scop);
void	destructScop(t_scop *scop);

#endif