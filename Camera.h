#ifndef CAMERA_H
#define CAMERA_H

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement{FORWARD, BACKWARD, LEFT, RIGHT};

const GLfloat YAW        = -90.0f;
const GLfloat PITCH      =  0.0f;
const GLfloat SPEED      =  3.0f;
const GLfloat SENSITIVTY =  0.25f;
const GLfloat ZOOM       =  45.0f;

class Camera{
public:
	glm::vec3 Position, Front, Up, Right, WorldUp;
	GLfloat Yaw, Pitch, MovementSpeed, MouseSensitivity, Zoom;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
  	GLfloat yaw = YAW, GLfloat pitch = PITCH);
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
	GLfloat upX, GLfloat upY, GLfloat upZ,
       	GLfloat yaw, GLfloat pitch);
	glm::mat4 GetViewMatrix();
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime, GLboolean FPS = false);
	void ProcessMouseMovement(GLfloat xOff, GLfloat yOff, GLboolean constrainPitch = true);
	void ProcessMouseScroll(GLfloat yOff);
private:
	void updateCameraVectors();

};

#endif
