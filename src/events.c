#include "events.h"

static void	processKeyboard(t_scop *scop, t_keys key)
{
	float	velocity = scop->camera.movementSpeed * scop->deltaTime;
	t_vector3	front = scop->camera.front;
	front.y = 0;
	if (key == FORWARD || key == BACKWARD)
		normalizev3(&front);
	t_vector3	right = scop->camera.right;
	right.y = 0;
	if (key == LEFT || key == RIGHT)
		normalizev3(&right);
	if (key == FORWARD)
		sumvec3(&scop->camera.position, front, velocity);
	else if (key == BACKWARD)
		sumvec3(&scop->camera.position, front, -velocity);
	else if (key == LEFT)
		sumvec3(&scop->camera.position, right, -velocity);
	else if (key == RIGHT)
		sumvec3(&scop->camera.position, right, velocity);
	else if (key == SPACE)
		sumvec3(&scop->camera.position, scop->camera.worldUp, velocity);
	else if (key == SHIFT)
		sumvec3(&scop->camera.position, scop->camera.worldUp, -velocity);
	else if (key == CTRLP)
		scop->camera.movementSpeed = 5.0f;
	else if (key == CTRLR)
		scop->camera.movementSpeed = 2.5f;
}

static void	processMouse(t_camera* camera, float xoffset, float yoffset)
{
	const float maxpitch = M_PI_2 - M_PI / 90.0f;
	xoffset *= camera->mouseSensitivity;
	yoffset *= camera->mouseSensitivity;

	xoffset *= M_PI / 180.0f;
	yoffset *= M_PI / 180.0f;

	camera->yaw += xoffset;
	camera->pitch += yoffset;
	while (camera->yaw > 2.0f * M_PI)
		camera->yaw -= 2 * M_PI;
	while (camera->yaw < 0.0f)
		camera->yaw += 2 * M_PI;
	if (camera->pitch > maxpitch)
		camera->pitch = maxpitch;
	if (camera->pitch < -maxpitch)
		camera->pitch = -maxpitch;
	updateCameraVectors(camera);
}

void processInput(t_scop *scop)
{
	static char firstMouse;
	static char tPressed;
	static char yPressed;

	if (glfwGetKey(scop->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(scop->window, 1);

	if (glfwGetKey(scop->window, GLFW_KEY_W) == GLFW_PRESS)
		processKeyboard(scop, FORWARD);
	if (glfwGetKey(scop->window, GLFW_KEY_S) == GLFW_PRESS)
		processKeyboard(scop, BACKWARD);
	if (glfwGetKey(scop->window, GLFW_KEY_A) == GLFW_PRESS)
		processKeyboard(scop, LEFT);
	if (glfwGetKey(scop->window, GLFW_KEY_D) == GLFW_PRESS)
		processKeyboard(scop, RIGHT);
	if (glfwGetKey(scop->window, GLFW_KEY_SPACE) == GLFW_PRESS)
		processKeyboard(scop, SPACE);
	if (glfwGetKey(scop->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		processKeyboard(scop, SHIFT);
	if (glfwGetKey(scop->window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		processKeyboard(scop, CTRLP);
	if (glfwGetKey(scop->window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
		processKeyboard(scop, CTRLR);
	if (glfwGetKey(scop->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(scop->window, GL_TRUE);

	if (glfwGetKey(scop->window, GLFW_KEY_LEFT) == GLFW_PRESS)
		scop->rotation.y -= M_PI / 4 * scop->deltaTime;
	if (glfwGetKey(scop->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		scop->rotation.y += M_PI / 4 * scop->deltaTime;
	if (glfwGetKey(scop->window, GLFW_KEY_UP) == GLFW_PRESS)
		scop->rotation.x -= M_PI / 4 * scop->deltaTime;
	if (glfwGetKey(scop->window, GLFW_KEY_DOWN) == GLFW_PRESS)
		scop->rotation.x += M_PI / 4 * scop->deltaTime;
	if (glfwGetKey(scop->window, GLFW_KEY_PERIOD) == GLFW_PRESS)
		scop->rotation.z -= M_PI / 4 * scop->deltaTime;
	if (glfwGetKey(scop->window, GLFW_KEY_COMMA) == GLFW_PRESS)
		scop->rotation.z += M_PI / 4 * scop->deltaTime;

	if (!tPressed && glfwGetKey(scop->window, GLFW_KEY_T) == GLFW_PRESS)
	{
		scop->texSign *= -1;
		tPressed = 1;
	}
	if (tPressed && glfwGetKey(scop->window, GLFW_KEY_T) == GLFW_RELEASE)
		tPressed = 0;

	if (!yPressed && glfwGetKey(scop->window, GLFW_KEY_Y) == GLFW_PRESS)
	{
		scop->tempTex++;
		scop->tempTex %= (scop->texCount + 1);
		yPressed = 1;
	}
	if (yPressed && glfwGetKey(scop->window, GLFW_KEY_Y) == GLFW_RELEASE)
		yPressed = 0;

	if (glfwGetMouseButton(scop->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (firstMouse == 0)
		{
			glfwSetInputMode(scop->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			glfwSetCursorPos(scop->window, (screenWidth / 2), (screenHeight / 2));
			firstMouse = 1;
		}
		double mouseX;
		double mouseY;
		glfwGetCursorPos(scop->window, &mouseX, &mouseY);
		processMouse(&scop->camera, mouseX - (screenWidth / 2), (screenHeight / 2) - mouseY);
		glfwSetCursorPos(scop->window, (screenWidth / 2), (screenHeight / 2));
	}
	else if (glfwGetMouseButton(scop->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(scop->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstMouse = 0;
	}

	if (scop->rotation.x > M_PI * 2.0f)
		scop->rotation.x -= M_PI * 2.0f;
	else if (scop->rotation.x < 0.0f)
		scop->rotation.x += M_PI * 2.0f;
	if (scop->rotation.y > M_PI * 2.0f)
		scop->rotation.y -= M_PI * 2.0f;
	else if (scop->rotation.y < 0.0f)
		scop->rotation.y += M_PI * 2.0f;
	if (scop->rotation.z > M_PI * 2.0f)
		scop->rotation.z -= M_PI * 2.0f;
	else if (scop->rotation.z < 0.0f)
		scop->rotation.z += M_PI * 2.0f;
}
