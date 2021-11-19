#ifndef RENDERER_H
# define RENDERER_H

# ifndef _WIN32
#  include <glad/glad.h>
# else
#  include <GL/glew.h>
#  include <GL/gl.h>
# endif
# include <GLFW/glfw3.h>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# define _USE_MATH_DEFINES
# include <math.h>
# include "camera.h"

#define ASSERT(x) if (!(x)) printf("Error!\n");
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall())

void	GLClearError();
int		GLLogCall();
unsigned int	MakeBuffer(unsigned int type, unsigned int size, void* data);
GLFWwindow	*GLInit();

#endif
