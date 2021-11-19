#ifndef SCOP_H
# define SCOP_H

# include "renderer.h"
# include "scop_struct.h"
# include "camera.h"
# include "vector/vector3.h"
# include "utils.h"
# include "mesh.h"
# include "texture.h"
# include "events.h"
# include "Shaders.h"
# include "UI.h"

void	initScop(t_scop *scop, const char* objName);
void	setLocations(t_scop *scop);
void	calculateDeltaTime(t_scop* scop);
void	destructScop(t_scop *scop);
void	initMainVertexArray(t_scop *scop);
void	initInterfaceVertexArray(t_scop	*scop);
void	mainLoop(t_scop *scop);

#endif