#include "objParser.h"

static size_t skipWhiteSpace(char* str, size_t i)
{
	while (str[i] == ' ' || str[i] == '\r')
		++i;
	return i;
}

static size_t skipNum(char* str, size_t i)
{
	while (str[i] == '.' || (str[i] >= '0' && str[i] <= '9') || str[i] == '-')
		++i;
	return i;
}

static void	resizeFaces(t_obj *ret, size_t *capacity, size_t size)
{
	*capacity *= 2;
	ret->faces.normalIdx = realloc(ret->faces.normalIdx, size * sizeof(unsigned int));
	ret->faces.texCoordIdx = realloc(ret->faces.texCoordIdx, size * sizeof(unsigned int));
	ret->faces.vertIdx = realloc(ret->faces.vertIdx, size * sizeof(unsigned int));
	if (!ret->faces.normalIdx || !ret->faces.texCoordIdx || !ret->faces.vertIdx)
		exit(EXIT_FAILURE);
}

static void	parseFaces(FILE* f, char* str, t_obj* ret, size_t* size)
{
	size_t	capacity = 32;
	size_t	i;
	int		count;

	ret->faces.normalIdx = malloc(capacity * sizeof(unsigned int));
	ret->faces.texCoordIdx = malloc(capacity * sizeof(unsigned int));
	ret->faces.vertIdx = malloc(capacity * sizeof(unsigned int));
	if (!ret->faces.normalIdx || !ret->faces.texCoordIdx || !ret->faces.vertIdx)
		exit(EXIT_FAILURE);
	while (fscanf(f, "%[^\n]", str) >= 0)
	{
		count = 0;
		if (str[0] == 'f' && str[1] == ' ')
		{
			i = skipWhiteSpace(str, 1);
			while (str[i])
			{
				if (*size + 3 >= capacity)
					resizeFaces(ret, &capacity, capacity * 2);
				if (count > 2)
				{
					ret->faces.vertIdx[*size] = ret->faces.vertIdx[*size - count];
					ret->faces.vertIdx[*size + 1] = ret->faces.vertIdx[*size - 1];
					ret->faces.texCoordIdx[*size] = ret->faces.texCoordIdx[*size - count];
					ret->faces.texCoordIdx[*size + 1] = ret->faces.texCoordIdx[*size - 1];
					ret->faces.normalIdx[*size] = ret->faces.normalIdx[*size - count];
					ret->faces.normalIdx[*size + 1] = ret->faces.normalIdx[*size - 1];
					*size += 2;
				}
				ret->faces.vertIdx[*size] = atoi(&(str[i])) - 1;
				ret->faces.normalIdx[*size] = 0;
				ret->faces.texCoordIdx[*size] = 0;
				i = skipNum(str, i);
				if (str[i] == '/')
				{
					if (str[++i] != '/')
						ret->faces.texCoordIdx[*size] = atoi(&(str[i])) - 1;
					i = skipNum(str, i);
				}
				if (str[i] == '/')
				{
					++i;
					ret->faces.normalIdx[*size] = atoi(&(str[i])) - 1;
					i = skipNum(str, i);
				}
				i = skipWhiteSpace(str, i);
				++(*size);
				++count;
			}
		}
		fscanf(f, "%c", (char*)&i);
	}
	resizeFaces(ret, &capacity, *size);
}

static float	*parseVertices(FILE *f, char *str, const char *ident, size_t *size)
{
	size_t	capacity = 32;
	float	*vertices = malloc(capacity * sizeof(float));
	size_t	i;

	if (!vertices)
		return 0;
	while (fscanf(f, "%[^\n]", str) >= 0)
	{
		if (!strncmp(str, ident, strlen(ident)))
		{
			int	count = 0;
			i = skipWhiteSpace(str, strlen(ident) - 1);
			while (str[i])
			{
				if (*size >= capacity)
				{
					capacity *= 2;
					vertices = realloc(vertices, capacity * sizeof(float));
					if (!vertices)
						exit(EXIT_FAILURE);
				}
				vertices[(*size)++] = atof(&(str[i]));
				i = skipNum(str, i);
				i = skipWhiteSpace(str, i);
				++count;
			}
			if ((!strcmp(ident, "v ") && count != 3) || (!strcmp(ident, "vn ") && count != 3) || (!strcmp(ident, "vt ") && count != 2))
			{
				printf("obj file corrupted!\n");
				exit(EXIT_FAILURE);
			}
		}
		fscanf(f, "%c", (char*)&i);
	}
	if (*size == 0)
	{
		free(vertices);
		return 0;
	}
	vertices = realloc(vertices, (*size) * sizeof(float));
	if (!vertices)
		exit(EXIT_FAILURE);
	return vertices;
}

// �������� ��������� �� �������
t_obj	*parseObj(const char *name)
{
	char filePath[50];
	memset(filePath, 0, 50);
	sprintf(filePath, "res/objects/%s.obj", name);
	FILE* f = fopen(filePath, "rb");
	if (!f)
	{
		printf("Unable to open obj file!\n");
		exit(EXIT_FAILURE);
	}
	char* str = malloc(512);
	t_obj* ret = malloc(sizeof(t_obj));

	if (!str || !ret)
		exit(EXIT_FAILURE);
	memset(ret, 0, sizeof(t_obj));
	ret->vertices = parseVertices(f, str, "v ", &ret->vertSize);
	fseek(f, 0, SEEK_SET);
	ret->texCoords = parseVertices(f, str, "vt ", &ret->tCoordSize);
	fseek(f, 0, SEEK_SET);
	ret->normals = parseVertices(f, str, "vn ", &ret->normSize);
	fseek(f, 0, SEEK_SET);
	parseFaces(f, str, ret, &ret->faceSize);
	free(str);
	if (!ret->vertices || !ret->faces.normalIdx || !ret->faces.texCoordIdx || !ret->faces.vertIdx)
		exit(EXIT_FAILURE);
	return ret;
}

void	freeObj(t_obj* obj)
{
	free(obj->vertices);
	free(obj->texCoords);
	free(obj->normals);
	free(obj->faces.vertIdx);
	free(obj->faces.texCoordIdx);
	free(obj->faces.normalIdx);
	free(obj);
}
