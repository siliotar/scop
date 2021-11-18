#include "scop.h"

void	initScop(t_scop *scop, const char *objName)
{
	scop->screenWidth = 1366;
	scop->screenHeight = 768;
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
	createMatrix3(&(scop->rotmat), 1.0f);

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
	addLocation(scop->objectShader, &scop->locations.lightPos, "u_lightPos");
	addLocation(scop->objectShader, &scop->locations.lightColor, "u_lightColor");
	addLocation(scop->objectShader, &scop->locations.perspective, "perspective");
	addLocation(scop->objectShader, &scop->locations.view, "view");
	addLocation(scop->objectShader, &scop->locations.rotmat, "rotmat");
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
	if (scop->tempTex > 0)
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, scop->textures[scop->tempTex - 1].id));
	}
	else
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, scop->textures[0].id));
	}

	GLCall(glUniform4f(scop->locations.perspective, M_PI_4, (float)scop->screenWidth / scop->screenHeight, 0.1f, 100.0f));

	t_vector3 tmp = scop->camera.position;
	sumvec3(&tmp, scop->camera.front, -1.0f);
	lookat(&scop->lookAt, scop->camera.position, tmp, scop->camera.up);
	GLCall(glUniformMatrix4fv(scop->locations.view, 1, GL_FALSE, (const GLfloat*)&scop->lookAt));
	GLCall(glUniformMatrix3fv(scop->locations.rotmat, 1, GL_FALSE, (const GLfloat*)&scop->rotmat));

	GLCall(glUniform1f(scop->locations.texTrans, scop->texTrans));

	GLCall(glDrawElements(GL_TRIANGLES, scop->mesh.vCount, GL_UNSIGNED_INT, 0));

	scop->texTrans += 0.1 * scop->texSign * scop->deltaTime;
	if (scop->texTrans > 1.0f)
		scop->texTrans = 1.0f;
	if (scop->texTrans < 0.0f)
		scop->texTrans = 0.0f;
	scop->rotmat = muliplyMat3(scop->rotmat, getyrot(0.5f * M_PI));
	scop->rotmat = muliplyMat3(getzrot(scop->rotation.z), muliplyMat3(getyrot(scop->rotation.y), \
		muliplyMat3(getxrot(scop->rotation.x), scop->rotmat)));
	scop->rotmat = muliplyMat3(scop->rotmat, getyrot(-0.5f * M_PI));
	setVector3(&(scop->rotation), 0.0f, 0.0f, 0.0f);
}

void	checkResize(t_scop *scop)
{
	int	w, h;

	w = scop->screenWidth;
	h = scop->screenHeight;
	glfwGetWindowSize(scop->window, &scop->screenWidth , &scop->screenHeight);
	if (w != scop->screenWidth || h != scop->screenWidth)
		glViewport(0, 0, scop->screenWidth, scop->screenHeight);
}

void	mainLoop(t_scop *scop)
{
	while (!glfwWindowShouldClose(scop->window))
	{
		checkResize(scop);
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
