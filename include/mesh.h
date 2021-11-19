#ifndef MESH_H
# define MESH_H

# include "vector/vector3.h"
# include "vector/vector2.h"
# include "objParser.h"

typedef struct		s_vertex
{
	t_vector3		position;
	t_vector2		texCoords;
	t_vector3		normal;
}					t_vertex;

typedef struct		s_mesh
{
	t_vertex		*vertices;
	unsigned int	*idxs;
	size_t			vCount;
}					t_mesh;

void	objToMesh(t_mesh* mesh, t_obj* obj);
void	createMesh(t_mesh* mesh, const char* path);
void	freeMesh(t_mesh* mesh);

#endif