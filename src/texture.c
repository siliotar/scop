#include "texture.h"

GLenum	getFormat(int bpp)
{
	if (bpp == 1)
		return GL_RED;
	else if (bpp == 3)
		return GL_RGB;
	return GL_RGBA;
}

static void	bindTexture(t_texture *tex)
{
	static int	count;
	tex->id = 0;

	GLCall(glGenTextures(1, &tex->id));
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glBindTexture(GL_TEXTURE_2D, tex->id));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, getFormat(tex->bpp), tex->width, tex->height, 0, getFormat(tex->bpp), GL_UNSIGNED_BYTE, tex->data));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));

	GLCall(glBindTexture(GL_TEXTURE_2D, count++));
}

void	parseTexture(t_scop *scop)
{
	for (size_t i = 0; i < scop->texCount; i++)
	{
		bindTexture(&scop->textures[i]);
		free(scop->textures[i].data);
	}
	bindTexture(&scop->interf.font);
	bindTexture(&scop->interf.menu);
	free(scop->interf.menu.data);
	bindTexture(&scop->interf.circle);
	free(scop->interf.circle.data);
}

void	loadTexture(t_texture *texture, const char *path, int isDefault)
{
	texture->data = read_bmp(path, &(texture->width), &(texture->height), &(texture->bpp));
	if (!texture->data && isDefault)
	{
		printf("Failed to open texture with the path \"%s\"!\n", path);
		exit(EXIT_FAILURE);
	}
}

void	loadTextures(t_scop *scop, const char *name)
{
	char	filePath[50];

	memset(filePath, 0, 50);
	for (int i = 0; i < 4; i++)
	{
		sprintf(filePath, "res/textures/default%i.bmp", i);
		loadTexture(&(scop->textures[i]), filePath, 1);
	}
	loadTexture(&(scop->interf.font), "res/textures/Font.bmp", 1);
	loadTexture(&(scop->interf.menu), "res/textures/menu.bmp", 1);
	loadTexture(&(scop->interf.circle), "res/textures/circle.bmp", 1);
	memset(filePath, 0, 50);
	sprintf(filePath, "res/textures/%s.bmp", name);
	loadTexture(&(scop->textures[4]), filePath, 0);
	if (scop->textures[4].data)
		scop->texCount = 5;
	else
		scop->texCount = 4;
}
