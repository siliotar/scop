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

static int	hitInCheckBox(t_scop *scop, char slot, double mouseX, double mouseY)
{
	return (mouseX > (scop->screenWidth - 41.0f)) && (mouseX < (scop->screenWidth - 17.0f)) \
		&& (mouseY > (slot * 48.0f + 12.0f)) && (mouseY < (slot * 48.0f + 36.0f));
}

static int	hitInSlider(t_scop *scop, char slot, double mouseX, double mouseY)
{
	return (scop->interf.showColorChanger) && (mouseX > (scop->screenWidth - 226.0f)) \
		&& (mouseX < (scop->screenWidth - 16.0f)) && (mouseY > (scop->screenHeight - (slot * 48.0f) + 14.0f)) \
		&& (mouseY < (scop->screenHeight - (slot * 48.0f) + 34.0f));
}

static void	processInterface(t_scop *scop, int *flags, double mouseX, double mouseY)
{
	if (!(*flags & INTERFACEPRESSERD))
	{
		char	slot = mouseY / 48.0;
		*flags |= INTERFACEPRESSERD;
		if (mouseX > scop->screenWidth - 66 && mouseY < 48)
		{
			if (scop->interf.menuIsOpen)
				scop->interf.showColorChanger = 0;
			scop->interf.menuIsOpen = !scop->interf.menuIsOpen;
		}
		else if (slot == 1 && hitInCheckBox(scop, slot, mouseX, mouseY))
			scop->interf.fpsIsOn = !scop->interf.fpsIsOn;
		else if (slot == 2 && hitInCheckBox(scop, slot, mouseX, mouseY))
		{
			scop->interf.VSync = !scop->interf.VSync;
			glfwSwapInterval(scop->interf.VSync);
		}
		else if (slot == 3)
		{
			scop->interf.showColorChanger = 1;
			scop->interf.colorToChange = &scop->backgroundColor;
		}
		else if (slot == 4)
		{
			scop->interf.showColorChanger = 1;
			scop->interf.colorToChange = &scop->modelColor;
		}
		else if (slot == 5)
		{
			scop->interf.showColorChanger = 1;
			scop->interf.colorToChange = &scop->lightColor;
		}
		else if (hitInSlider(scop, 3, mouseX, mouseY))
			*flags |= REDSLIDERPRESSERD;
		else if (hitInSlider(scop, 2, mouseX, mouseY))
			*flags |= GREENSLIDERPRESSERD;
		else if (hitInSlider(scop, 1, mouseX, mouseY))
			*flags |= BLUESLIDERPRESSERD;
	}
	float value = (mouseX - (scop->screenWidth - 216.0f)) / 190.0f;
	if (value < 0.0f)
		value = 0.0f;
	else if (value > 1.0f)
		value = 1.0f;
	if (*flags & REDSLIDERPRESSERD)
		scop->interf.colorToChange->x = value;
	else if (*flags & GREENSLIDERPRESSERD)
		scop->interf.colorToChange->y = value;
	else if (*flags & BLUESLIDERPRESSERD)
		scop->interf.colorToChange->z = value;
}

void processInput(t_scop *scop)
{
	static int	flags;

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

	if (!(flags & TPRESSERD) && glfwGetKey(scop->window, GLFW_KEY_T) == GLFW_PRESS)
	{
		scop->texSign *= -1;
		flags |= TPRESSERD;
	}
	if (flags & TPRESSERD && glfwGetKey(scop->window, GLFW_KEY_T) == GLFW_RELEASE)
		flags &= ~TPRESSERD;

	if (!(flags & YPRESSERD) && glfwGetKey(scop->window, GLFW_KEY_Y) == GLFW_PRESS)
	{
		scop->tempTex++;
		scop->tempTex %= (scop->texCount + 1);
		flags |= YPRESSERD;
	}
	if (flags & YPRESSERD && glfwGetKey(scop->window, GLFW_KEY_Y) == GLFW_RELEASE)
		flags &= ~YPRESSERD;

	if (glfwGetMouseButton(scop->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		double mouseX;
		double mouseY;
		glfwGetCursorPos(scop->window, &mouseX, &mouseY);
		if (!(flags & MOUSEPRESSERD) && ((!scop->interf.menuIsOpen && mouseX > scop->screenWidth - 66 && mouseY < 60) || \
			(scop->interf.menuIsOpen && mouseX > scop->screenWidth - 266)))
		{
			processInterface(scop, &flags, mouseX, mouseY);
		}
		else if (!(flags & INTERFACEPRESSERD))
		{
			if (!(flags & MOUSEPRESSERD))
			{
				glfwSetInputMode(scop->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				mouseX = scop->screenWidth / 2;
				mouseY = scop->screenHeight / 2;
				flags |= MOUSEPRESSERD;
			}
			processMouse(&scop->camera, mouseX - (scop->screenWidth / 2), (scop->screenHeight / 2) - mouseY);
			glfwSetCursorPos(scop->window, (scop->screenWidth / 2), (scop->screenHeight / 2));
		}
	}
	else if (glfwGetMouseButton(scop->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(scop->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		flags &= ~MOUSEPRESSERD;
		flags &= ~INTERFACEPRESSERD;
		flags &= ~REDSLIDERPRESSERD;
		flags &= ~GREENSLIDERPRESSERD;
		flags &= ~BLUESLIDERPRESSERD;
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
