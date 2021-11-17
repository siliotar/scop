#include "scop.h"

void	initScop(t_scop *scop, const char *objName)
{
	scop->interf.showColorChanger = 0;
	scop->interf.menuIsOpen = 0;
	scop->interf.fpsIsOn = 1;
	scop->interf.VSync = 1;
	scop->frames = 0;
	scop->rotation.x = 0.0f;
	scop->rotation.y = 0.0f;
	scop->rotation.z = 0.0f;
	scop->texTrans = 1.0f;
	scop->texSign = 1;
	scop->tempTex = 0;
	scop->texCount = 0;
	scop->backgroundColor.x = 0.52f;
	scop->backgroundColor.y = 0.52f;
	scop->backgroundColor.z = 0.52f;
	scop->modelColor.x = 0.0f;
	scop->modelColor.y = 0.2f;
	scop->modelColor.z = 0.8f;
	scop->lightColor.x = 1.0f;
	scop->lightColor.y = 1.0f;
	scop->lightColor.z = 1.0f;
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

	printf("%s\n\n\n", glGetString(GL_VERSION));

	setupCam(&scop->camera);
}

static void	addLocation(GLuint shader, GLint *loc, const char *name)
{
	GLCall(*loc = glGetUniformLocation(shader, name));
	ASSERT(*loc != -1);
}

void	setLocations(t_scop *scop)
{
	addLocation(scop->objectShader, &scop->locations.color, "u_Color");
	addLocation(scop->objectShader, &scop->locations.tempTexture, "u_SimpleColor");
	addLocation(scop->objectShader, &scop->locations.rotation, "rotation");
	addLocation(scop->objectShader, &scop->locations.lightPos, "u_lightPos");
	addLocation(scop->objectShader, &scop->locations.lightColor, "u_lightColor");
	addLocation(scop->objectShader, &scop->locations.perspective, "perspective");
	addLocation(scop->objectShader, &scop->locations.view, "view");
	addLocation(scop->objectShader, &scop->locations.texTrans, "u_texTrans");
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
	GLCall(glDeleteVertexArrays(1, &scop->interfaceArray.vao));
	GLCall(glDeleteBuffers(1, &scop->interfaceArray.vbo));
	GLCall(glDeleteProgram(scop->interfaceShader));
	for (size_t i = 0; i < scop->texCount; i++)
	{
		GLCall(glDeleteTextures(1, &scop->textures[i].id));
	}
	free(scop->interf.font.data);
	GLCall(glDeleteTextures(1, &scop->interf.font.id));
	GLCall(glDeleteTextures(1, &scop->interf.menu.id));
	GLCall(glDeleteTextures(1, &scop->interf.circle.id));
	glfwDestroyWindow(scop->window);
	glfwTerminate();
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

	addLocation(scop->interfaceShader, &scop->locations.interfColor, "u_Color");
	addLocation(scop->interfaceShader, &scop->locations.interfSimpleColor, "u_SimpleColor");
}

static void	drawScene(t_scop *scop)
{
	GLCall(glBindVertexArray(scop->mainArray.vao));
	GLCall(glUseProgram(scop->objectShader));

	GLCall(glUniform3f(scop->locations.lightColor, scop->lightColor.x, scop->lightColor.y, scop->lightColor.z));
	GLCall(glUniform4f(scop->locations.color, scop->modelColor.x, scop->modelColor.y, scop->modelColor.z, 1.0f));
	GLCall(glUniform1i(scop->locations.tempTexture, scop->tempTex));
	GLCall(glUniform3f(scop->locations.rotation, scop->rotation.x, scop->rotation.y, scop->rotation.z));
	if (scop->tempTex > 0)
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, scop->textures[scop->tempTex - 1].id));
	}
	else
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, scop->textures[0].id));
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
	float	size = 16.0f * scale;
	float	squareH = size / screenHeight * 2.0f;
	float	squareW = size / screenWidth * 2.0f;
	float	fontH = 1.0f / 8;
	float	fontW = 1.0f / 16;
	x = x / screenWidth * 2.0f - 1.0f;
	y = y / screenHeight * 2.0f - 1.0f;
	GLCall(glBindTexture(GL_TEXTURE_2D, scop->interf.font.id));
	for (size_t i = 0; i < strlen(text); i++)
	{
		if (text[i] == ' ')
			continue ;
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

		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));

		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6););
	}
}

static void	drawImage(float x, float y, float scale, t_texture *tex)
{
	float	squareH = (float)tex->height / screenHeight * scale * 2.0f;
	float	squareW = (float)tex->width / screenWidth * scale * 2.0f;
	x = x / screenWidth * 2.0f - 1.0f;
	y = y / screenHeight * 2.0f - 1.0f;

	float vertices[6][4] = {
		{ x, y, 0.0f, 0.0f },
		{ x + squareW, y, 1.0f, 0.0f },
		{ x + squareW, y + squareH, 1.0f, 1.0f },
		{ x + squareW, y + squareH, 1.0f, 1.0f },
		{ x, y + squareH, 0.0f, 1.0f },
		{ x, y, 0.0f, 0.0f }
	};
	GLCall(glBindTexture(GL_TEXTURE_2D, tex->id));

	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));

	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6););
}

static void drawRect(t_scop *scop, float x1, float y1, float x2, float y2, float r, float g, float b)
{
	GLCall(glUniform1i(scop->locations.interfSimpleColor, 1));
	GLCall(glUniform4f(scop->locations.interfColor, r, g, b, 1.0f));

	x1 = x1 / screenWidth * 2.0f - 1.0f;
	y1 = y1 / screenHeight * 2.0f - 1.0f;
	x2 = x2 / screenWidth * 2.0f - 1.0f;
	y2 = y2 / screenHeight * 2.0f - 1.0f;

	float vertices[6][4] = {
		{ x1, y1, 0.0f, 0.0f },
		{ x2, y1, 1.0f, 0.0f },
		{ x2, y2, 1.0f, 1.0f },
		{ x2, y2, 1.0f, 1.0f },
		{ x1, y2, 0.0f, 1.0f },
		{ x1, y1, 0.0f, 0.0f }
	};

	GLCall(glBindTexture(GL_TEXTURE_2D, scop->interf.font.id));

	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));

	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));

	GLCall(glUniform1i(scop->locations.interfSimpleColor, 0));
}

static void	drawSlider(t_scop *scop, float x1, float y1, float x2, float y2, float r, float g, float b, float value)
{
	GLCall(glUniform1i(scop->locations.interfSimpleColor, 1));
	GLCall(glUniform4f(scop->locations.interfColor, 0.1f, 0.1f, 0.1f, 0.0f));

	float	h = y2 - y1;
	float	leftRectW = x1 + h / 2 + (x2 - x1 - h) * value;

	float	scale = h / (float)scop->interf.circle.height;

	drawImage(x1 + (x2 - x1 - h) * value, y1, scale, &scop->interf.circle);

	GLCall(glUniform1i(scop->locations.interfSimpleColor, 1));
	GLCall(glUniform4f(scop->locations.interfColor, r, g, b, 0.0f));

	h /= 2;
	leftRectW = (x2 - x1 - 2 * h) * value;
	scale = h / (float)scop->interf.circle.height;

	drawImage(x1 + h / 2, y1 + h / 2, scale, &scop->interf.circle);
	drawRect(scop, x1 + h, y1 + h / 2, x1 + h + leftRectW, y2 - h / 2, r, g, b);

	GLCall(glUniform1i(scop->locations.interfSimpleColor, 1));
	GLCall(glUniform4f(scop->locations.interfColor, 0.4f, 0.4f, 0.4f, 0.0f));

	drawImage(x2 - h - h / 2, y1 + h / 2, scale, &scop->interf.circle);
	drawRect(scop, x1 + h + leftRectW, y1 + h / 2, x2 - h, y2 - h / 2, 0.4f, 0.4f, 0.4f);

	GLCall(glUniform1i(scop->locations.interfSimpleColor, 0));
}

static void	drawColorChanger(t_scop *scop, t_vector3 *value)
{
	GLCall(glUniform1i(scop->locations.interfSimpleColor, 1));
	GLCall(glUniform4f(scop->locations.interfColor, 1.0f, 1.0f, 1.0f, 0.0f));
	drawText(scop, 1110.0f, 111.0f, "R", 1.0f);
	drawText(scop, 1110.0f, 63.0f, "G", 1.0f);
	drawText(scop, 1110.0f, 15.0f, "B", 1.0f);
	drawSlider(scop, 1140.0f, 110.0f, 1350.0f, 130.0f, 1.0f, 0.0f, 0.0f, value->x);
	drawSlider(scop, 1140.0f, 62.0f, 1350.0f, 82.0f, 0.0f, 1.0f, 0.0f, value->y);
	drawSlider(scop, 1140.0f, 14.0f, 1350.0f, 34.0f, 0.0f, 0.0f, 1.0f, value->z);
}

static void	drawInterface(t_scop *scop)
{
	static float	lastFPStime;
	static int		FPS;
	char	buff[50];
	GLCall(glBindVertexArray(scop->interfaceArray.vao));
	GLCall(glUseProgram(scop->interfaceShader));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, scop->interfaceArray.vbo));
	if (glfwGetTime() - lastFPStime > 1.0f)
	{
		FPS = (float)scop->frames / (glfwGetTime() - lastFPStime);
		scop->frames = 0;
		lastFPStime = glfwGetTime();
	}
	if (scop->interf.fpsIsOn)
	{
		sprintf(buff, "fps: %i", FPS);
		GLCall(glUniform1i(scop->locations.interfSimpleColor, 1));
		GLCall(glUniform4f(scop->locations.interfColor, 1.0f, 1.0f, 1.0f, 0.0f));
		drawText(scop, 20.0f, 748.0f, buff, 1.0f);
	}
	GLCall(glUniform1i(scop->locations.interfSimpleColor, 0));
	drawImage(1310.0f, 722.0f, 0.035f, &scop->interf.menu);
	if (scop->interf.menuIsOpen)
	{
		GLCall(glUniform1i(scop->locations.interfSimpleColor, 1));
		GLCall(glUniform4f(scop->locations.interfColor, 1.0f, 1.0f, 1.0f, 0.0f));
		sprintf(buff, "FPS counter   %c", scop->interf.fpsIsOn ? 'X' : ' ');
		drawText(scop, 1110.0f, 687.0f, buff, 1.0f);
		sprintf(buff, "VSync         %c", scop->interf.VSync ? 'X' : ' ');
		drawText(scop, 1110.0f, 639.0f, buff, 1.0f);
		drawText(scop, 1110.0f, 591.0f, "Background color", 1.0f);
		drawText(scop, 1110.0f, 543.0f, "Model color", 1.0f);
		drawText(scop, 1110.0f, 495.0f, "Light color", 1.0f);
		if (scop->interf.showColorChanger)
			drawColorChanger(scop, scop->interf.colorToChange);
		drawRect(scop, 1326.0f, 684.0f, 1350.0f, 708.0f, 0.1f, 0.1f, 0.1f);
		drawRect(scop, 1326.0f, 636.0f, 1350.0f, 660.0f, 0.1f, 0.1f, 0.1f);
		drawRect(scop, 1100.0f, 672.0f, 1366.0f, 720.0f, 0.4f, 0.4f, 0.4f);
		drawRect(scop, 1100.0f, 576.0f, 1366.0f, 624.0f, 0.4f, 0.4f, 0.4f);
		drawRect(scop, 1100.0f, 480.0f, 1366.0f, 528.0f, 0.4f, 0.4f, 0.4f);
		//drawRect(scop, 1100.0f, 384.0f, 1366.0f, 432.0f, 0.4f, 0.4f, 0.4f);
		//drawRect(scop, 1100.0f, 288.0f, 1366.0f, 336.0f, 0.4f, 0.4f, 0.4f);
		//drawRect(scop, 1100.0f, 192.0f, 1366.0f, 240.0f, 0.4f, 0.4f, 0.4f);
		//drawRect(scop, 1100.0f, 96.0f, 1366.0f, 144.0f, 0.4f, 0.4f, 0.4f);
		//drawRect(scop, 1100.0f, 0.0f, 1366.0f, 48.0f, 0.4f, 0.4f, 0.4f);
		drawRect(scop, 1100.0f, 0.0f, 1366.0f, 768.0f, 0.45f, 0.45f, 0.45f);
	}
}

void	mainLoop(t_scop *scop)
{
	while (!glfwWindowShouldClose(scop->window))
	{
		calculateDeltaTime(scop);
		GLCall(glClearColor(scop->backgroundColor.x, scop->backgroundColor.y, scop->backgroundColor.z, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		processInput(scop);

		drawScene(scop);

		drawInterface(scop);

		scop->frames++;

		GLCall(glfwSwapBuffers(scop->window));
		GLCall(glfwPollEvents());
	}
}
