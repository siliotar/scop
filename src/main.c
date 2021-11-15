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

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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

void	displayUsage()
{
	printf("Moving\t\t\t\t\t\tW, A, S, D\n");
	printf("Move up\t\t\t\t\t\tSPACE\n");
	printf("Move down\t\t\t\t\tLEFT SHIFT\n");
	printf("Speed up\t\t\t\t\tLEFT CTRL\n");
	printf("Rotate the model around the x-axis\t\tUP, DOWN\n");
	printf("Rotate the model around the y-axis\t\tLEFT, RIGHT\n");
	printf("Rotate the model around the z-axis\t\t<, >\n");
	printf("Enable display of triangles\t\t\tT\n");
	printf("Switch texture\t\t\t\t\tY\n");
	printf("Exit\t\t\t\t\t\tESC\n\n");
	printf("Hold LMB to rotate the camera\n\n");
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Usage: ./scop name_of_object_file\n");
		return 0;
	}

	displayUsage();

	t_scop	scop;
	initScop(&scop);

	t_obj* obj = parseObj(argv[1]);
	t_mesh	mesh;
	objToMesh(&mesh, obj);
	printf("Triangles: %ld\n", obj->faceSize / 3);
	freeObj(obj);

	loadTextures(&scop, argv[1]);

	scop.window = GLInit();

	printf("%s\n", glGetString(GL_VERSION));

	GLCall(glGenVertexArrays(1, &scop.vao));
	GLCall(glBindVertexArray(scop.vao));

	GLCall(glEnableVertexArrayAttrib(scop.vao, 0));

	scop.vbo = MakeBuffer(GL_ARRAY_BUFFER, mesh.vCount * sizeof(t_vertex), mesh.vertices);
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0));
	glEnableVertexAttribArray(0);

	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
	glEnableVertexAttribArray(1);

	GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))));
	glEnableVertexAttribArray(2);

	scop.ibo = MakeBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.vCount, mesh.idxs);

	freeMesh(&mesh);

	scop.shader = CreateShader("res/shaders/VertexBasic.shader", "res/shaders/FragmentBasic.shader");

	GLCall(glUseProgram(scop.shader));

	setLocations(&scop);

	GLCall(glUniform3f(scop.locations.lightPos, 50.0f, 50.0f, 50.0f));
	GLCall(glUniform3f(scop.locations.lightColor, 1.0f, 1.0f, 1.0f));

	parseTexture(&scop);

	glEnable(GL_DEPTH_TEST);

	setupCam(&scop.camera);

	size_t	frames = 0;
	float	start = glfwGetTime();

	GLCall(glBindVertexArray(scop.vao));

	while (!glfwWindowShouldClose(scop.window))
	{
		calculateDeltaTime(&scop);
		GLCall(glClearColor(0.9f, 1.0f, 0.4f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		GLCall(glUniform4f(scop.locations.color, scop.textureColor.x, scop.textureColor.y, scop.textureColor.z, 1.0f));
		GLCall(glUniform1i(scop.locations.tempTexture, scop.tempTex));
		GLCall(glUniform3f(scop.locations.rotation, scop.rotation.x, scop.rotation.y, scop.rotation.z));
		if (scop.tempTex > 0)
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, scop.texids[scop.tempTex - 1]));
		}
		else
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, scop.texids[0]));
		}

		processInput(&scop);

		GLCall(glUniform4f(scop.locations.perspective, M_PI_4, (float)screenWidth / screenHeight, 0.1f, 100.0f));

		t_vector3 tmp = scop.camera.position;
		sumvec3(&tmp, scop.camera.front, -1.0f);
		lookat(&scop.lookAt, scop.camera.position, tmp, scop.camera.up);
		GLCall(glUniformMatrix4fv(scop.locations.view, 1, GL_FALSE, (const GLfloat *)&scop.lookAt));

		GLCall(glUniform1f(scop.locations.texTrans, scop.texTrans));
		
		GLCall(glDrawElements(GL_TRIANGLES, mesh.vCount, GL_UNSIGNED_INT, 0));

		GLCall(glfwSwapBuffers(scop.window));
		GLCall(glfwPollEvents());

		frames++;
		scop.texTrans += 0.1 * scop.texSign * scop.deltaTime;
		if (scop.texTrans > 1.0f)
			scop.texTrans = 1.0f;
		if (scop.texTrans < 0.0f)
			scop.texTrans = 0.0f;
	}

	printf("FPS = %f\n", (float)frames / (glfwGetTime() - start));

	destructScop(&scop);
	glfwTerminate();
	return 0;
}
