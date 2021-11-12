#include "renderer.h"

void	GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

int	GLLogCall()
{
	GLenum error;
	int ret = 1;
	while ((error = glGetError()))
	{
		printf("[OpenGL error] (%u)\n", error);
		ret = 0;
	}
	return ret;
}
