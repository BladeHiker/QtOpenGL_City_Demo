#include "Camera.h"

Camera::Camera(QVector3D position, QVector3D up, float yaw, float pitch)
	: Forward(QVector3D(0.0f, -5.0f, -1.0f))
	  , MovementSpeed(SPEED)
	  , Mouse_Sensiticity(SENSITIVITY)
	  , Zoom(ZOOM)
{
	this->Position = position;
	this->World_up = up;
	this->Yaw = yaw;
	this->Pitch = pitch;
	UpdateCameraVectors();
}

Camera::Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch)
	: Forward(QVector3D(0.0f, -5.0f, -1.0f))
	  , MovementSpeed(SPEED)
	  , Mouse_Sensiticity(SENSITIVITY)
	  , Zoom(ZOOM)
{
	this->Position = QVector3D(pos_x, pos_y, pos_z);
	this->World_up = QVector3D(up_x, up_y, up_z);
	this->Yaw = yaw;
	this->Pitch = pitch;
	UpdateCameraVectors();
}

Camera::~Camera()
{
}

QMatrix4x4 Camera::GetViewMatrix()
{
	QMatrix4x4 m;
	m.lookAt(Position, Position + Forward, Up);
	return m;
}

//对应键盘移动事件
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Position += Forward * velocity;
	if (direction == BACKWARD)
		Position -= Forward * velocity;
	if (direction == LEFT)
		Position -= Right * velocity;
	if (direction == RIGHT)
		Position += Right * velocity;
	if (direction == UP)
		Position += Up * velocity;
	if (direction == DOWN)
		Position -= Up * velocity;
}

//对应鼠标移动事件
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= Mouse_Sensiticity;
	yoffset *= Mouse_Sensiticity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	UpdateCameraVectors();
}

//对应鼠标滚轮事件
void Camera::ProcessMouseScroll(float yoffset)
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}


void Camera::UpdateCameraVectors()
{
	QVector3D front;
	front.setX(cos(PI / 180 * Yaw) * cos(PI / 180 * Pitch));
	front.setY(sin(PI / 180 * Pitch));
	front.setZ(sin(PI / 180 * Yaw) * cos(PI / 180 * Pitch));
	Forward = front.normalized();
	Right = QVector3D::crossProduct(Forward, World_up).normalized();
	Up = QVector3D::crossProduct(Right, Forward).normalized();
}
