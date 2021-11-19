#ifndef SHADERS_H
# define SHADERS_H

# ifndef _WIN32
#  include <glad/glad.h>
# else
#  include <GL/glew.h>
#  include <GL/gl.h>
# endif
# include <GLFW/glfw3.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "fileReader.h"

unsigned int	CreateShader(const char* vertexShaderPath, const char* fragmentShaderPath);

#endif
