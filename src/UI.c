#include "UI.h"

static void	drawText(t_scop *scop, float x, float y, const char *text, float scale)
{
	float	size = 16.0f * scale;
	float	squareH = size / scop->screenHeight * 2.0f;
	float	squareW = size / scop->screenWidth * 2.0f;
	float	fontH = 1.0f / 8;
	float	fontW = 1.0f / 16;
	x = x / scop->screenWidth * 2.0f - 1.0f;
	y = y / scop->screenHeight * 2.0f - 1.0f;
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

static void	drawImage(float x, float y, float scale, t_texture *tex, t_scop *scop)
{
	float	squareH = (float)tex->height / scop->screenHeight * scale * 2.0f;
	float	squareW = (float)tex->width / scop->screenWidth * scale * 2.0f;
	x = x / scop->screenWidth * 2.0f - 1.0f;
	y = y / scop->screenHeight * 2.0f - 1.0f;

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

static void	drawRect(t_scop *scop, float x1, float y1, float x2, float y2, float r, float g, float b)
{
	GLCall(glUniform1i(scop->locations.interfSimpleColor, 1));
	GLCall(glUniform4f(scop->locations.interfColor, r, g, b, 1.0f));

	x1 = x1 / scop->screenWidth * 2.0f - 1.0f;
	y1 = y1 / scop->screenHeight * 2.0f - 1.0f;
	x2 = x2 / scop->screenWidth * 2.0f - 1.0f;
	y2 = y2 / scop->screenHeight * 2.0f - 1.0f;

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

	drawImage(x1 + (x2 - x1 - h) * value, y1, scale, &scop->interf.circle, scop);

	GLCall(glUniform1i(scop->locations.interfSimpleColor, 1));
	GLCall(glUniform4f(scop->locations.interfColor, r, g, b, 0.0f));

	h /= 2;
	leftRectW = (x2 - x1 - 2 * h) * value;
	scale = h / (float)scop->interf.circle.height;

	drawImage(x1 + h / 2, y1 + h / 2, scale, &scop->interf.circle, scop);
	drawRect(scop, x1 + h, y1 + h / 2, x1 + h + leftRectW, y2 - h / 2, r, g, b);

	GLCall(glUniform1i(scop->locations.interfSimpleColor, 1));
	GLCall(glUniform4f(scop->locations.interfColor, 0.4f, 0.4f, 0.4f, 0.0f));

	drawImage(x2 - h - h / 2, y1 + h / 2, scale, &scop->interf.circle, scop);
	drawRect(scop, x1 + h + leftRectW, y1 + h / 2, x2 - h, y2 - h / 2, 0.4f, 0.4f, 0.4f);

	GLCall(glUniform1i(scop->locations.interfSimpleColor, 0));
}

static void	drawColorChanger(t_scop *scop, t_vector3 *value)
{
	GLCall(glUniform1i(scop->locations.interfSimpleColor, 1));
	GLCall(glUniform4f(scop->locations.interfColor, 1.0f, 1.0f, 1.0f, 0.0f));
	drawText(scop, scop->screenWidth - 256.0f, 111.0f, "R", 1.0f);
	drawText(scop, scop->screenWidth - 256.0f, 63.0f, "G", 1.0f);
	drawText(scop, scop->screenWidth - 256.0f, 15.0f, "B", 1.0f);
	drawSlider(scop, scop->screenWidth - 226.0f, 110.0f, scop->screenWidth - 16.0f, 130.0f, 1.0f, 0.0f, 0.0f, value->x);
	drawSlider(scop, scop->screenWidth - 226.0f, 62.0f, scop->screenWidth - 16.0f, 82.0f, 0.0f, 1.0f, 0.0f, value->y);
	drawSlider(scop, scop->screenWidth - 226.0f, 14.0f, scop->screenWidth - 16.0f, 34.0f, 0.0f, 0.0f, 1.0f, value->z);
}

static void	drawCheckbox(t_scop *scop, const char *text, char slot, char value)
{
	static char buff[50];
	float x = scop->screenWidth - 266.0f;
	float y = scop->screenHeight - 48.0f * slot;

	sprintf(buff, "%-14s%c", text, value ? 'X' : ' ');
	drawText(scop, x + 10.0f, y + 15.0f, buff, 1.0f);
	drawRect(scop, x + 226.0f, y + 12.0f, x + 250.0f, y + 36.0f, 0.1f, 0.1f, 0.1f);
	if (slot % 2 == 0)
		drawRect(scop, x, y, scop->screenWidth, y + 48.0f, 0.4f, 0.4f, 0.4f);
}

static void	drawButton(t_scop *scop, const char *text, char slot)
{
	float x = scop->screenWidth - 266.0f;
	float y = scop->screenHeight - 48.0f * slot;

	drawText(scop, x + 10.0f, y + 15.0f, text, 1.0f);
	if (slot % 2 == 0)
		drawRect(scop, x, y, scop->screenWidth, y + 48.0f, 0.4f, 0.4f, 0.4f);
}

void	drawInterface(t_scop *scop)
{
	static float	lastFPStime;
	static int		FPS;
	char			buff[16];

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
		drawText(scop, 20.0f, scop->screenHeight - 20.0f, buff, 1.0f);
	}
	GLCall(glUniform1i(scop->locations.interfSimpleColor, 0));
	drawImage(scop->screenWidth - 56.0f, scop->screenHeight - 46.0f, 0.035f, &scop->interf.menu, scop);
	if (scop->interf.menuIsOpen)
	{
		GLCall(glUniform1i(scop->locations.interfSimpleColor, 1));
		GLCall(glUniform4f(scop->locations.interfColor, 1.0f, 1.0f, 1.0f, 0.0f));
		drawCheckbox(scop, "FPS counter", 2, scop->interf.fpsIsOn);
		drawCheckbox(scop, "VSunc", 3, scop->interf.VSync);
		drawButton(scop, "Background color", 4);
		drawButton(scop, "Model color", 5);
		drawButton(scop, "Light color", 6);
		if (scop->interf.showColorChanger)
			drawColorChanger(scop, scop->interf.colorToChange);
		drawRect(scop, scop->screenWidth - 266.0f, 0.0f, scop->screenWidth, scop->screenHeight, 0.45f, 0.45f, 0.45f);
	}
}
