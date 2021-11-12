#ifndef SHADERS_H
# define SHADERS_H

# include <GL/glew.h>
# include <GL/gl.h>
# include <GLFW/glfw3.h>
# include <stdio.h>
# include <stdlib.h>

# include "fileReader.h"

unsigned int	CreateShader(const char* vertexShaderPath, const char* fragmentShaderPath);

#endif
