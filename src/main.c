#include "renderer.h"
#include "Shaders.h"
#include "matrix/matrix4.h"
#include "objParser.h"
#include "events.h"
#include "mesh.h"
#include "texture.h"

static unsigned int	MakeBuffer(unsigned int type, unsigned int size, void* data)
{
	unsigned int	buffer;

	GLCall(glGenBuffers(1, &buffer));
	GLCall(glBindBuffer(type, buffer));
	GLCall(glBufferData(type, size, data, GL_STATIC_DRAW));
	return buffer;
}

static GLFWwindow* GLInit()
{
	GLFWwindow* window;
	if (!glfwInit())
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//mac
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow(screenWidth, screenHeight, "scop", 0, 0);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	return window;
}

int main(int argc, char** argv)
{
	if (argc < 2)
		return 0;
	t_scop	scop;
	initScop(&scop);

	t_obj* obj = parseObj(argv[1]);
	t_mesh	mesh;
	objToMesh(&mesh, obj);

	loadTextures(&scop, argv[1]);

	GLFWwindow* window = GLInit();

	printf("%s\n", glGetString(GL_VERSION));

	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	GLCall(glEnableVertexArrayAttrib(vao, 0)); // Some weird stuff

	unsigned int	buffer = MakeBuffer(GL_ARRAY_BUFFER, mesh.vCount * sizeof(t_vertex), mesh.vertices);
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0));
	glEnableVertexAttribArray(0);

	if (obj->texCoords)
	{
		GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
		glEnableVertexAttribArray(1);
	}

	unsigned int	ibo = MakeBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.vCount, mesh.idxs);

	printf("Triangles: %ld\n", obj->faceSize / 3);
	freeMesh(&mesh);
	freeObj(obj);

	unsigned int	shader = CreateShader("res/shaders/VertexBasic.shader", "res/shaders/FragmentBasic.shader");

	GLCall(glUseProgram(shader));

	GLCall(int colorLocation = glGetUniformLocation(shader, "u_Color"));
	ASSERT(colorLocation != -1);

	GLCall(int rotLocation = glGetUniformLocation(shader, "rotation"));
	ASSERT(rotLocation != -1);

	GLCall(glBindVertexArray(0));
	GLCall(glUseProgram(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	GLuint *texids = parseTexture(&scop);

	glEnable(GL_DEPTH_TEST);

	GLCall(int perspectiveLoc = glGetUniformLocation(shader, "perspective"));
	ASSERT(perspectiveLoc != -1);
	GLCall(int viewLoc = glGetUniformLocation(shader, "view"));
	ASSERT(viewLoc != -1);
	size_t frames = 0;

	GLCall(int texTransLocation = glGetUniformLocation(shader, "u_texTrans"));
	ASSERT(texTransLocation != -1);

	t_camera camera;
	setupCam(&camera);
	t_matrix4 matrix;

	float	currentFrame;
	float	lastFrame = 0.0f;
	float	deltaTime = 0.0f;
	float	start = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		GLCall(glClearColor(0.9f, 1.0f, 0.4f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		GLCall(glUseProgram(shader));

		//GLCall(glUniform4f(colorLocation, 0.0f, 0.2f, 0.8f, 1.0f));
		GLCall(glUniform4f(colorLocation, 0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glUniform3f(rotLocation, scop.anglex, scop.angley, scop.anglez));
		GLCall(glBindTexture(GL_TEXTURE_2D, texids[scop.tempTex]));

		processInput(window, &camera, deltaTime, &scop);

		GLCall(glUniform4f(perspectiveLoc, M_PI_4, (float)screenWidth / screenHeight, 0.1f, 100.0f));

		t_vector3 tmp = camera.position;
		sumvec3(&tmp, camera.front, -1.0f);
		lookat(&matrix, camera.position, tmp, camera.up);
		GLCall(glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (const GLfloat *)&matrix));

		GLCall(glUniform1f(texTransLocation, scop.texTrans));

		GLCall(glBindVertexArray(vao));
		//GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

		
		GLCall(glDrawElements(GL_TRIANGLES, mesh.vCount, GL_UNSIGNED_INT, 0));

		scop.angley += M_PI / 4 * deltaTime;
		if (scop.angley > M_PI * 2.0f)
			scop.angley -= M_PI * 2.0f;
		GLCall(glfwSwapBuffers(window));
		GLCall(glfwPollEvents());
		frames++;
		scop.texTrans += 0.1 * scop.texSign * deltaTime;
		if (scop.texTrans > 1.0f)
			scop.texTrans = 1.0f;
		if (scop.texTrans < 0.0f)
			scop.texTrans = 0.0f;
	}

	printf("FPS = %f\n", (float)frames / (glfwGetTime() - start));

	GLCall(glDeleteVertexArrays(1, &vao));
	GLCall(glDeleteBuffers(1, &buffer));
	GLCall(glDeleteBuffers(1, &ibo));
	GLCall(glDeleteProgram(shader));
	GLCall(glDeleteTextures(2, texids));
	free(texids);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
