#include "mesh.h"

void	objToMesh(t_mesh *mesh, t_obj *obj)
{
	unsigned int	i;

	i = 0;
	mesh->vertices = malloc(obj->faceSize * sizeof(t_vertex));
	mesh->idxs = malloc(obj->faceSize * sizeof(unsigned int));
	if (!mesh->idxs || !mesh->vertices)
	{
		free(mesh->vertices);
		free(mesh->idxs);
		return ;
	}
	while (i < obj->faceSize)
	{
		memcpy(&(mesh->vertices[i].position), &(obj->vertices[obj->faces.vertIdx[i] * 3]), sizeof(float) * 3);
		if (obj->texCoords)
			memcpy(&(mesh->vertices[i].texCoords), &(obj->texCoords[obj->faces.texCoordIdx[i] * 2]), sizeof(float) * 2);
		else
			memset(&(mesh->vertices[i].texCoords), 0, sizeof(float) * 2);
		if (obj->normals)
			memcpy(&(mesh->vertices[i].normal), &(obj->normals[obj->faces.normalIdx[i] * 3]), sizeof(float) * 3);
		else
			memset(&(mesh->vertices[i].normal), 0, sizeof(float) * 3);
		mesh->idxs[i] = i;
		++i;
	}
	mesh->vCount = i;
}

void	freeMesh(t_mesh* mesh)
{
	free(mesh->vertices);
	free(mesh->idxs);
}
