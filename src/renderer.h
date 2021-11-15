#ifndef RENDERER_H
# define RENDERER_H

# include <GL/glew.h>
# include <GL/gl.h>
# include <GLFW/glfw3.h>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# define _USE_MATH_DEFINES
# include <math.h>
# include "camera.h"
# include "scop.h"

# define screenWidth 1366
# define screenHeight 768

#define ASSERT(x) if (!(x)) printf("Error!\n");
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall())

void	GLClearError();
int		GLLogCall();

#endif
