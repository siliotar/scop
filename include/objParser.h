#ifndef OBJPARSER_H
# define OBJPARSER_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "fileReader.h"

typedef struct s_face
{
	unsigned int *vertIdx;
	unsigned int *texCoordIdx;
	unsigned int *normalIdx;
}t_face;

typedef struct s_obj
{
	float* vertices;
	float* texCoords;
	float* normals;
	t_face faces;
	size_t	vertSize;
	size_t	tCoordSize;
	size_t	normSize;
	size_t	faceSize;
}t_obj;

t_obj* parseObj(const char* name);
void	freeObj(t_obj* obj);

#endif