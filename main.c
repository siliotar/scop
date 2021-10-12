#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

static unsigned int	CompileShader(unsigned int type, const char *source)
{
	unsigned int	id = glCreateShader(type);
	glShaderSource(id, 1, &source, 0);
	glCompileShader(id);

	int	result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int	length = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char	message[length];
		glGetShaderInfoLog(id, length, &length, message);
		printf("Failed to compile shader\n%s\n", message);
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int	CreateShader(const char *vertexShader, const char *fragmentShader)
{
	unsigned int	program = glCreateProgram();
	unsigned int	vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int	fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(int argc, char **argv)
{
	GLFWwindow	*window;
	if (!glfwInit())
	{
		glfwTerminate();
		return -1;
	}
	window = glfwCreateWindow(640, 480, "Hello world", 0, 0);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return -1;
	}

	float	positions[6] = {
		-0.5f, -0.5f,
		-0.5f, 0.5f,
		0.5f, 0.5f
	};

	unsigned int	buffer;

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(0, 0); // Some wierd stuff
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	char	*vertexShader = \
		"#version 330 core\n" \
		"\n" \
		"layout(location = 0) in vec4 position;\n" \
		"\n" \
		"void main()\n" \
		"{\n" \
		"	gl_Position = position;\n" \
		"}\n";

	char	*fragmentShader = \
		"#version 330 core\n" \
		"\n" \
		"layout(location = 0) out vec4 color;\n" \
		"\n" \
		"void main()\n" \
		"{\n" \
		"	color = vec4(1.0, 0.0, 0.0, 1.0);\n" \
		"}\n";

	unsigned int	shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}