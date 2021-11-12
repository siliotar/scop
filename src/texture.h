#ifndef TEXTURE_H
# define TEXTURE_H

# include "renderer.h"
# include "stb_image.h"
# include <string.h>

GLuint	*parseTexture(t_scop* scop);
void	loadTextures(t_scop* scop, const char* name);

#endif
