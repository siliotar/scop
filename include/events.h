#ifndef EVENTS_H
# define EVENTS_H

# define MOUSEPRESSERD			0x1
# define INTERFACEPRESSERD		0x2
# define TPRESSERD				0x4
# define YPRESSERD				0x8
# define REDSLIDERPRESSERD		0x10
# define GREENSLIDERPRESSERD	0x20
# define BLUESLIDERPRESSERD		0x40

# include "renderer.h"
# include "scop_struct.h"

void processInput(t_scop *scop);

#endif
