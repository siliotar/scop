#ifndef SCOP_STRUCT_H
# define SCOP_STRUCT_H

# include "renderer.h"
# include "mesh.h"

typedef struct s_texture
{
	int				bpp;
	int				width;
	int				height;
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

typedef struct s_vertexArray
{
	GLuint			vao;
	GLuint			vbo;
	GLuint			ibo;
}t_vertexArray;

typedef struct s_scop
{
	t_vector3		rotation;
	char			texSign;
	unsigned char	tempTex;
	float			texTrans;
	t_texture		textures[5];
	t_texture		fontTexture;
	unsigned char	texCount;
	t_vector3		textureColor;
	GLuint			texids[5];
	GLuint			fontTextureId;
	GLFWwindow		*window;
	t_vertexArray	mainArray;
	t_vertexArray	interfaceArray;
	GLuint			objectShader;
	GLuint			interfaceShader;
	t_locations		locations;
	float			deltaTime;
	t_camera		camera;
	t_matrix4		lookAt;
	t_mesh			mesh;
	size_t			frames;
}t_scop;


#endif
