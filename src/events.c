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
		if (!(flags & MOUSEPRESSERD) && ((!scop->interf.menuIsOpen && mouseX > 1300 && mouseY < 60) || \
			(scop->interf.menuIsOpen && mouseX > 1100)))
		{
			if (!(flags & INTERFACEPRESSERD))
			{
				flags |= INTERFACEPRESSERD;
				if (mouseX > 1300 && mouseY < 48)
				{
					if (scop->interf.menuIsOpen)
						scop->interf.showColorChanger = 0;
					scop->interf.menuIsOpen = !scop->interf.menuIsOpen;
				}
				else if (mouseX > 1325.0f && mouseY > 60.0f && mouseX < 1349.0f && mouseY < 84.0f)
					scop->interf.fpsIsOn = !scop->interf.fpsIsOn;
				else if (mouseX > 1325.0f && mouseY > 108.0f && mouseX < 1349.0f && mouseY < 132.0f)
				{
					scop->interf.VSync = !scop->interf.VSync;
					glfwSwapInterval(scop->interf.VSync);
				}
				else if (mouseX > 1100.0f && mouseY > 144.0f && mouseY < 192.0f)
				{
					scop->interf.showColorChanger = 1;
					scop->interf.colorToChange = &scop->backgroundColor;
				}
				else if (mouseX > 1100.0f && mouseY > 192.0f && mouseY < 240.0f)
				{
					scop->interf.showColorChanger = 1;
					scop->interf.colorToChange = &scop->modelColor;
				}
				else if (mouseX > 1100.0f && mouseY > 240.0f && mouseY < 288.0f)
				{
					scop->interf.showColorChanger = 1;
					scop->interf.colorToChange = &scop->lightColor;
				}
				else if (scop->interf.showColorChanger && mouseX > 1140.0f && mouseX < 1350.0f && mouseY > 638.0f && mouseY < 658.0f)
					flags |= REDSLIDERPRESSERD;
				else if (scop->interf.showColorChanger && mouseX > 1140.0f && mouseX < 1350.0f && mouseY > 686.0f && mouseY < 706.0f)
					flags |= GREENSLIDERPRESSERD;
				else if (scop->interf.showColorChanger && mouseX > 1140.0f && mouseX < 1350.0f && mouseY > 734.0f && mouseY < 754.0f)
					flags |= BLUESLIDERPRESSERD;
			}
			float value = (mouseX - 1150.0f) / 190.0f;
			if (value < 0.0f)
				value = 0.0f;
			else if (value > 1.0f)
				value = 1.0f;
			if (flags & REDSLIDERPRESSERD)
				scop->interf.colorToChange->x = value;
			else if (flags & GREENSLIDERPRESSERD)
				scop->interf.colorToChange->y = value;
			else if (flags & BLUESLIDERPRESSERD)
				scop->interf.colorToChange->z = value;
		}
		else if (!(flags & INTERFACEPRESSERD))
		{
			if (!(flags & MOUSEPRESSERD))
			{
				glfwSetInputMode(scop->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				mouseX = screenWidth / 2;
				mouseY = screenHeight / 2;
				flags |= MOUSEPRESSERD;
			}
			processMouse(&scop->camera, mouseX - (screenWidth / 2), (screenHeight / 2) - mouseY);
			glfwSetCursorPos(scop->window, (screenWidth / 2), (screenHeight / 2));
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
