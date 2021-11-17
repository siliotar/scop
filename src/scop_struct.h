#ifndef SCOP_STRUCT_H
# define SCOP_STRUCT_H

# include "renderer.h"
# include "mesh.h"

typedef struct s_texture
{
	GLuint			id;
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
	GLint			interfColor;
	GLint			interfSimpleColor;
}t_locations;

typedef struct s_vertexArray
{
	GLuint			vao;
	GLuint			vbo;
	GLuint			ibo;
}t_vertexArray;

typedef struct s_interface
{
	t_texture		font;
	t_texture		menu;
	t_texture		circle;
	char			menuIsOpen;
	char			fpsIsOn;
	char			VSync;
	char			showColorChanger;
	t_vector3		*colorToChange;
}t_interface;

typedef struct s_scop
{
	t_vector3		rotation;
	char			texSign;
	unsigned char	tempTex;
	float			texTrans;
	t_texture		textures[5];
	unsigned char	texCount;
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
	t_interface		interf;
	t_vector3		modelColor;
	t_vector3		backgroundColor;
	t_vector3		lightColor;
}t_scop;


#endif
