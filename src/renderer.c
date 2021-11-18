#include "renderer.h"

void	GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

int	GLLogCall()
{
	GLenum error;
	int ret = 1;
	while ((error = glGetError()))
	{
		printf("[OpenGL error] (%u)\n", error);
		ret = 0;
	}
	return ret;
}

unsigned int	MakeBuffer(unsigned int type, unsigned int size, void* data)
{
	unsigned int	buffer;

	GLCall(glGenBuffers(1, &buffer));
	GLCall(glBindBuffer(type, buffer));
	GLCall(glBufferData(type, size, data, GL_STATIC_DRAW));
	return buffer;
}

GLFWwindow	*GLInit()
{
	GLFWwindow* window;
	if (!glfwInit())
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	window = glfwCreateWindow(1366, 768, "scop", 0, 0);
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
