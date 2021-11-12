#ifndef CAMERA_H
# define CAMERA_H

# define _USE_MATH_DEFINES
# include <math.h>
# include "vector/vector3.h"
# include "matrix/matrix4.h"

typedef enum keys
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	SPACE,
	SHIFT,
	CTRLP,
	CTRLR
}t_keys;

typedef struct	s_camera
{
	t_vector3	position;
	t_vector3	front;
	t_vector3	up;
	t_vector3	right;
	t_vector3	worldUp;
	float		yaw;
	float		pitch;
	float		movementSpeed;
	float		mouseSensitivity;
	float		zoom;
}				t_camera;

void	setupCam(t_camera* camera);
void	processKeyboard(t_camera* camera, t_keys key, float deltaTime);
void	processMouse(t_camera* camera, float xoffset, float yoffset);
void	lookat(t_matrix4* mat, t_vector3 cameraPos, t_vector3 cameraTarget, t_vector3 cameraUp);

#endif
