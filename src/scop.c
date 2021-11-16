#include "scop.h"

void	initScop(t_scop *scop, const char *objName)
{
	scop->frames = 0;
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

	t_obj* obj = parseObj(objName);
	objToMesh(&scop->mesh, obj);
	printf("Triangles: %ld\n", obj->faceSize / 3);
	freeObj(obj);

	loadTextures(scop, objName);

	scop->window = GLInit();

	printf("%s\n", glGetString(GL_VERSION));

	setupCam(&scop->camera);
}

static void	addLocation(t_scop *scop, GLint *loc, const char *name)
{
	GLCall(*loc = glGetUniformLocation(scop->objectShader, name));
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
	GLCall(glDeleteVertexArrays(1, &scop->mainArray.vao));
	GLCall(glDeleteBuffers(1, &scop->mainArray.vbo));
	GLCall(glDeleteBuffers(1, &scop->mainArray.ibo));
	GLCall(glDeleteProgram(scop->objectShader));
	GLCall(glDeleteTextures(scop->texCount, scop->texids));
	glfwDestroyWindow(scop->window);
}

void	initMainVertexArray(t_scop *scop)
{
	GLCall(glGenVertexArrays(1, &scop->mainArray.vao));
	GLCall(glBindVertexArray(scop->mainArray.vao));

	GLCall(glEnableVertexArrayAttrib(scop->mainArray.vao, 0));

	scop->mainArray.vbo = MakeBuffer(GL_ARRAY_BUFFER, scop->mesh.vCount * sizeof(t_vertex), scop->mesh.vertices);
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0));
	glEnableVertexAttribArray(0);

	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
	glEnableVertexAttribArray(1);

	GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))));
	glEnableVertexAttribArray(2);

	scop->mainArray.ibo = MakeBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * scop->mesh.vCount, scop->mesh.idxs);

	freeMesh(&scop->mesh);

	scop->objectShader = CreateShader("res/shaders/VertexBasic.shader", "res/shaders/FragmentBasic.shader");

	GLCall(glUseProgram(scop->objectShader));

	setLocations(scop);

	GLCall(glUniform3f(scop->locations.lightPos, 50.0f, 50.0f, 50.0f));
	GLCall(glUniform3f(scop->locations.lightColor, 1.0f, 1.0f, 1.0f));

	parseTexture(scop);

	glEnable(GL_DEPTH_TEST);
}

void	initInterfaceVertexArray(t_scop	*scop)
{
	GLCall(glGenVertexArrays(1, &scop->interfaceArray.vao));
	GLCall(glBindVertexArray(scop->interfaceArray.vao));

	GLCall(glEnableVertexArrayAttrib(scop->interfaceArray.vao, 0));
	GLCall(glGenBuffers(1, &scop->interfaceArray.vbo));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, scop->interfaceArray.vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW));

	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(3);

	scop->interfaceShader = CreateShader("res/shaders/VertexInterface.shader", "res/shaders/FragmentInterface.shader");
}

static void	drawScene(t_scop *scop)
{
	GLCall(glBindVertexArray(scop->mainArray.vao));
	GLCall(glUseProgram(scop->objectShader));

	GLCall(glUniform4f(scop->locations.color, scop->textureColor.x, scop->textureColor.y, scop->textureColor.z, 1.0f));
	GLCall(glUniform1i(scop->locations.tempTexture, scop->tempTex));
	GLCall(glUniform3f(scop->locations.rotation, scop->rotation.x, scop->rotation.y, scop->rotation.z));
	if (scop->tempTex > 0)
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, scop->texids[scop->tempTex - 1]));
	}
	else
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, scop->texids[0]));
	}

	GLCall(glUniform4f(scop->locations.perspective, M_PI_4, (float)screenWidth / screenHeight, 0.1f, 100.0f));

	t_vector3 tmp = scop->camera.position;
	sumvec3(&tmp, scop->camera.front, -1.0f);
	lookat(&scop->lookAt, scop->camera.position, tmp, scop->camera.up);
	GLCall(glUniformMatrix4fv(scop->locations.view, 1, GL_FALSE, (const GLfloat*)&scop->lookAt));

	GLCall(glUniform1f(scop->locations.texTrans, scop->texTrans));

	GLCall(glDrawElements(GL_TRIANGLES, scop->mesh.vCount, GL_UNSIGNED_INT, 0));

	scop->texTrans += 0.1 * scop->texSign * scop->deltaTime;
	if (scop->texTrans > 1.0f)
		scop->texTrans = 1.0f;
	if (scop->texTrans < 0.0f)
		scop->texTrans = 0.0f;
}

static void	drawText(t_scop *scop, float x, float y, const char *text, float scale)
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, scop->interfaceArray.vbo));
	float	size = 32.0f * scale;
	float	squareH = size / screenHeight;
	float	squareW = size / screenWidth;
	float	fontH = size / scop->fontTexture.height;
	float	fontW = size / scop->fontTexture.width;
	x = x / screenWidth * 2.0f - 1.0f;
	y = y / screenHeight * 2.0f - 1.0f;
	for (size_t i = 0; i < strlen(text); i++)
	{
		int pos = text[i] - ' ';
		int line = pos / 16;
		pos %= 16;
		float xpos = (x + squareW * i);
		float ypos = y;
		float xl = pos * fontW;
		float yb = (7 - line) * fontH;
		float vertices[6][4] = {
			{ xpos, ypos, xl, yb },
			{ xpos + squareW, ypos, xl + fontW, yb },
			{ xpos + squareW, ypos + squareH, xl + fontW, yb + fontH },
			{ xpos + squareW, ypos + squareH, xl + fontW, yb + fontH },
			{ xpos, ypos + squareH, xl, yb + fontH },
			{ xpos, ypos, xl, yb }
		};
		GLCall(glBindTexture(GL_TEXTURE_2D, scop->fontTextureId));

		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));

		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6););
	}
}

static void	drawInterface(t_scop *scop)
{
	static float	lastFPStime;
	static int		FPS;
	char	buff[50];
	GLCall(glBindVertexArray(scop->interfaceArray.vao));
	GLCall(glUseProgram(scop->interfaceShader));
	if (glfwGetTime() - lastFPStime > 1.0f)
	{
		FPS = (float)scop->frames / (glfwGetTime() - lastFPStime);
		scop->frames = 0;
		lastFPStime = glfwGetTime();
	}
	memset(buff, 0, 50);
	sprintf(buff, "fps: %i", FPS);
	drawText(scop, 20.0f, 748.0f, buff, 1.0f);
}

void	mainLoop(t_scop *scop)
{
	while (!glfwWindowShouldClose(scop->window))
	{
		calculateDeltaTime(scop);
		GLCall(glClearColor(0.9f, 1.0f, 0.4f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		processInput(scop);

		drawScene(scop);

		drawInterface(scop);

		scop->frames++;

		GLCall(glfwSwapBuffers(scop->window));
		GLCall(glfwPollEvents());
	}
}
