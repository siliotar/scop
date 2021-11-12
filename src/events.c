#include "events.h"

void processInput(GLFWwindow* window, t_camera* camera, float deltaTime, t_scop *scop)
{
	static char firstMouse;
	static char tPressed;
	static char yPressed;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		processKeyboard(camera, FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		processKeyboard(camera, BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		processKeyboard(camera, LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		processKeyboard(camera, RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		processKeyboard(camera, SPACE, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		processKeyboard(camera, SHIFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		processKeyboard(camera, CTRLP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
		processKeyboard(camera, CTRLR, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (!tPressed && glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	{
		scop->texSign *= -1;
		tPressed = 1;
	}
	if (tPressed && glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE)
		tPressed = 0;

	if (!yPressed && glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
	{
		scop->tempTex++;
		scop->tempTex %= 2;
		yPressed = 1;
	}
	if (yPressed && glfwGetKey(window, GLFW_KEY_Y) == GLFW_RELEASE)
		yPressed = 0;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (firstMouse == 0)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			glfwSetCursorPos(window, (screenWidth / 2), (screenHeight / 2));
			firstMouse = 1;
		}
		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		processMouse(camera, mouseX - (screenWidth / 2), (screenHeight / 2) - mouseY);
		glfwSetCursorPos(window, (screenWidth / 2), (screenHeight / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstMouse = 0;
	}
}
