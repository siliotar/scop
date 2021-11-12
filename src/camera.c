#include "camera.h"

static void	updateCameraVectors(t_camera *camera)
{
	camera->front.x = cos(camera->yaw) * cos(camera->pitch);
	camera->front.y = sin(camera->pitch);
	camera->front.z = sin(camera->yaw) * cos(camera->pitch);

	normalizev3(&camera->front);
	crossv3(&camera->right, camera->front, camera->worldUp);
	normalizev3(&camera->right);
	crossv3(&camera->up, camera->right, camera->front);
	normalizev3(&camera->up);
}

void	setupCam(t_camera* camera)
{
	camera->position.x = 0.0f;
	camera->position.y = 0.0f;
	camera->position.z = 0.0f;
	camera->up.x = 0.0f;
	camera->up.y = 1.0f;
	camera->up.z = 0.0f;
	camera->worldUp = camera->up;
	camera->yaw = -M_PI_2;
	camera->pitch = 0.0f;
	camera->mouseSensitivity = 0.1f;
	camera->movementSpeed = 2.5f;
	updateCameraVectors(camera);
}

void	processKeyboard(t_camera *camera, t_keys key, float deltaTime)
{
	float	velocity = camera->movementSpeed * deltaTime;
	if (key == FORWARD)
		sumvec3(&camera->position, camera->front, velocity);
	else if (key == BACKWARD)
		sumvec3(&camera->position, camera->front, -velocity);
	else if (key == LEFT)
		sumvec3(&camera->position, camera->right, -velocity);
	else if (key == RIGHT)
		sumvec3(&camera->position, camera->right, velocity);
	else if (key == SPACE)
		sumvec3(&camera->position, camera->worldUp, velocity);
	else if (key == SHIFT)
		sumvec3(&camera->position, camera->worldUp, -velocity);
	else if (key == CTRLP)
		camera->movementSpeed = 5.0f;
	else if (key == CTRLR)
		camera->movementSpeed = 2.5f;
}

void	processMouse(t_camera *camera, float xoffset, float yoffset)
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

void	lookat(t_matrix4 *mat, t_vector3 cameraPos, t_vector3 cameraTarget, t_vector3 cameraUp)
{
	sumvec3(&cameraTarget, cameraPos, -1.0f);
	normalizev3(&cameraTarget);
	t_vector3	zaxis = cameraTarget;
	t_vector3	xaxis = cameraTarget;
	t_vector3	yaxis = cameraTarget;
	crossv3(&xaxis, cameraUp, zaxis);
	normalizev3(&xaxis);
	crossv3(&yaxis, zaxis, xaxis);

	mat->field[0].x = xaxis.x;	mat->field[0].y = yaxis.x;	mat->field[0].z = zaxis.x;	mat->field[0].w = 0.0f;
	mat->field[1].x = xaxis.y;	mat->field[1].y = yaxis.y;	mat->field[1].z = zaxis.y;	mat->field[1].w = 0.0f;
	mat->field[2].x = xaxis.z;	mat->field[2].y = yaxis.z;	mat->field[2].z = zaxis.z;	mat->field[2].w = 0.0f;
	mat->field[3].x = -dotvec3(xaxis, cameraPos);		mat->field[3].y = -dotvec3(yaxis, cameraPos);
	mat->field[3].z = -dotvec3(zaxis, cameraPos);		mat->field[3].w = 1.0f;
}
