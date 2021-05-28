#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <QOpenGLFunctions>
#include <QVector3D>
#include <QMatrix4x4>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.03f;
const float ZOOM = 45.0f;
const float PI = 3.14159f;

//摄像机移动方向
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera
{
public:
	QVector3D Position;
	QVector3D Forward;
	QVector3D Up;
	QVector3D Right;
	QVector3D World_up;

	float Yaw;
	float Pitch;

	float MovementSpeed;
	float Mouse_Sensiticity;
	float Zoom;
	bool flip_y = false;

	Camera(QVector3D position = QVector3D(0.0f, 1.0f, 5.0f), QVector3D up = QVector3D(0.0f, 1.0f, 0.0f),
	       float yaw = YAW, float pitch = PITCH);
	Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch);
	~Camera();

	QMatrix4x4 GetViewMatrix();
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(float yoffset);

private:
	void UpdateCameraVectors();
};

#endif
