#include "Shaders.h"

static char			*ParseShader(const char* filepath)
{
	char	*ret;

	ret = readFile(filepath);
	if (ret == NULL)
		glfwTerminate();
	return ret;
}

static unsigned int	CompileShader(unsigned int type, const char* source)
{
	unsigned int	id = glCreateShader(type);
	glShaderSource(id, 1, &source, 0);
	glCompileShader(id);

	int	result = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int	length = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char	message[512];
		glGetShaderInfoLog(id, length, &length, message);
		printf("Failed to compile shader\n%s\n", message);
		glDeleteShader(id);
		return 0;
	}
	return id;
}

unsigned int	CreateShader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	char* vertexShader = ParseShader(vertexShaderPath);
	if (vertexShader == NULL)
		exit(EXIT_FAILURE);

	char* fragmentShader = ParseShader(fragmentShaderPath);
	if (fragmentShader == NULL)
	{
		free(vertexShader);
		exit(EXIT_FAILURE);
	}

	unsigned int	program = glCreateProgram();
	unsigned int	vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int	fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	free(vertexShader);
	free(fragmentShader);

	return program;
}
