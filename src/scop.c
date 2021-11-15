#include "scop.h"

void	initScop(t_scop *scop)
{
	scop->rotation.x = 0.0f;
	scop->rotation.y = 0.0f;
	scop->rotation.z = 0.0f;
	scop->texTrans = 1.0f;
	scop->texSign = 1;
	scop->tempTex = 0;
	scop->texCount = 0;
	scop->textureColor.x = 0.0f;
	scop->textureColor.y = 0.2f;
	scop->textureColor.z = 0.8f;
	for (size_t i = 0; i < 5; i++)
		scop->textures[i].data = 0;
	scop->window = 0;
	scop->deltaTime = 0.0f;
}

static void	addLocation(t_scop *scop, GLint *loc, const char *name)
{
	GLCall(*loc = glGetUniformLocation(scop->shader, name));
	ASSERT(*loc != -1);
}

void	setLocations(t_scop *scop)
{
	addLocation(scop, &scop->locations.color, "u_Color");
	addLocation(scop, &scop->locations.tempTexture, "u_SimpleColor");
	addLocation(scop, &scop->locations.rotation, "rotation");
	addLocation(scop, &scop->locations.lightPos, "u_lightPos");
	addLocation(scop, &scop->locations.lightColor, "u_lightColor");
	addLocation(scop, &scop->locations.perspective, "perspective");
	addLocation(scop, &scop->locations.view, "view");
	addLocation(scop, &scop->locations.texTrans, "u_texTrans");
}

void	calculateDeltaTime(t_scop* scop)
{
	static float	lastFrame;
	float	currentFrame;

	currentFrame = glfwGetTime();
	scop->deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void	destructScop(t_scop *scop)
{
	GLCall(glDeleteVertexArrays(1, &scop->vao));
	GLCall(glDeleteBuffers(1, &scop->vbo));
	GLCall(glDeleteBuffers(1, &scop->ibo));
	GLCall(glDeleteProgram(scop->shader));
	GLCall(glDeleteTextures(scop->texCount, scop->texids));
	glfwDestroyWindow(scop->window);
}
