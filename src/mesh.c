#include "mesh.h"

static t_vector3	calcNormal(t_vector3 a, t_vector3 b, t_vector3 c)
{
	t_vector3	v1, v2, res;

	v1.x = a.x - b.x;
	v1.y = a.y - b.y;
	v1.z = a.z - b.z;

	v2.x = b.x - c.x;
	v2.y = b.y - c.y;
	v2.z = b.z - c.z;

	res.x = v1.y * v2.z - v1.z * v2.y;
	res.y = v1.z * v2.x - v1.x * v2.z;
	res.z = v1.x * v2.y - v1.y * v2.x;
	return res;
}

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
			memcpy(&(mesh->vertices[i].texCoords), &(obj->vertices[obj->faces.vertIdx[i] * 3]), sizeof(float) * 2);
		if (obj->normals)
			memcpy(&(mesh->vertices[i].normal), &(obj->normals[obj->faces.normalIdx[i] * 3]), sizeof(float) * 3);
		else if (i % 3 == 2)
		{
			t_vector3	norm = calcNormal(mesh->vertices[i - 2].position, mesh->vertices[i - 1].position, mesh->vertices[i].position);
			mesh->vertices[i - 2].normal = norm;
			mesh->vertices[i - 1].normal = norm;
			mesh->vertices[i].normal = norm;
		}
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
