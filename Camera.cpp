#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch){
	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	MovementSpeed=SPEED;
	MouseSensitivity = SENSITIVTY;
	Zoom = ZOOM;
	this->Position=position;
	this->WorldUp=up;
	this->Yaw=yaw;
	this->Pitch=pitch;
	updateCameraVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch){
	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	MovementSpeed=SPEED;
	MouseSensitivity = SENSITIVTY;
	Zoom = ZOOM;
	this->Position = glm::vec3(posX, posY, posZ);
	this->WorldUp = glm::vec3(upX, upY, upZ);
	this->Yaw = yaw;
	this->Pitch = pitch;
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix(){
	return glm::lookAt(Position, Position+Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime, GLboolean FPS){
	GLfloat velocity = MovementSpeed*deltaTime;
	if(direction==FORWARD) {
		Position.x+=Front.x*velocity;
		Position.z+=Front.z*velocity;
		if(!FPS) Position.y+=Front.y*velocity;
	}
	else if(direction==BACKWARD){
	       	Position.x-=Front.x*velocity;
		Position.z-=Front.z*velocity;
		if(!FPS)Position.y-=Front.y*velocity;
	}
	else if(direction==LEFT){
	       	Position.x+=Right.x*velocity;
		Position.z+=Right.z*velocity;
		if(!FPS)Position.y+=Right.y*velocity;
	}
	else if(direction==RIGHT){
	       	Position.x-=Right.x*velocity;
		Position.z-=Right.z*velocity;
		if(!FPS)Position.y-=Right.y*velocity;
	}
}

void Camera::ProcessMouseMovement(GLfloat xOff, GLfloat yOff, GLboolean constrainPitch){
	Yaw+=(xOff*MouseSensitivity);
	Pitch-=(yOff*MouseSensitivity);
	if(constrainPitch){
		if(Pitch<-89.0f) Pitch = -89.0f;
		else if(Pitch>89.0f) Pitch = 89.0f;}
	updateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat yOff){
	if(Zoom<1.0f) Zoom = 1.0f;
	else if(Zoom>90.0f) Zoom = 90.0f;
	else Zoom-=yOff;
}

void Camera::updateCameraVectors(){
	glm::vec3 front;
	front.x=cos(glm::radians(Pitch))*cos(glm::radians(Yaw));
	front.y=sin(glm::radians(Pitch));
	front.z=cos(glm::radians(Pitch))*sin(glm::radians(Yaw));
	Front=glm::normalize(front);
	Right=glm::normalize(glm::cross(WorldUp, Front));
	Up=glm::normalize(glm::cross(Front, Right));
}
