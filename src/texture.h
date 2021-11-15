#ifndef TEXTURE_H
# define TEXTURE_H

# include "renderer.h"
# include <string.h>
# include "bmp_reader.h"

void	parseTexture(t_scop* scop);
void	loadTextures(t_scop* scop, const char* name);

#endif
