#include "texture.h"

GLenum	getFormat(int bpp)
{
	if (bpp == 1)
		return GL_RED;
	else if (bpp == 3)
		return GL_RGB;
	return GL_RGBA;
}

GLuint	bindTexture(t_texture *tex)
{
	static int	count;
	GLuint	texid = 0;

	GLCall(glGenTextures(1, &texid));
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glBindTexture(GL_TEXTURE_2D, texid));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, getFormat(tex->bpp), tex->width, tex->height, 0, getFormat(tex->bpp), GL_UNSIGNED_BYTE, tex->data));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));

	GLCall(glBindTexture(GL_TEXTURE_2D, count++));

	return texid;
}

GLuint *parseTexture(t_scop *scop)
{
	GLuint	*texids = malloc(sizeof(GLuint) * 2);
	if (!texids)
		exit(EXIT_FAILURE);
	texids[0] = bindTexture(&scop->defaultTexture);
	if (!scop->customTexture.data)
		texids[1] = bindTexture(&scop->defaultTexture);
	else
		texids[1] = bindTexture(&scop->customTexture);

	stbi_image_free(scop->defaultTexture.data);
	if (scop->customTexture.data)
		stbi_image_free(scop->customTexture.data);

	return texids;
}

void	loadTexture(t_texture *texture, const char *path)
{
	texture->data = stbi_load(path, &(texture->width), &(texture->height), &(texture->bpp), 0);
}

void	loadTextures(t_scop *scop, const char *name)
{
	char	filePath[50];

	stbi_set_flip_vertically_on_load(1);
	loadTexture(&(scop->defaultTexture), "res/textures/brick.png");
	if (!scop->defaultTexture.data)
	{
		printf("Failed to open texture!\n");
		exit(EXIT_FAILURE);
	}
	memset(filePath, 0, 50);
	sprintf(filePath, "res/textures/%s.png", name);
	loadTexture(&(scop->customTexture), filePath);
}
