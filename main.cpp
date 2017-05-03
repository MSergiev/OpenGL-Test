#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Shader.h"
#include "Camera.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoff, double yoff);
void do_movement();

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024], firstMouse = true;
GLint WIDTH=800, HEIGHT=600;
GLfloat deltaTime = 0.0f, lastTime = 0.0f;
GLfloat lastX = WIDTH/2, lastY = HEIGHT/2;
glm::vec3 lightPos(1.2f, 1.0f, -3.0f);

int main(){
//INIT
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "TEST", nullptr, nullptr);
	if(window==nullptr){
		std::cout << "Falied to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glewExperimental = GL_TRUE;
	if(glewInit()!=GLEW_OK){
		std::cout << "Falied to initialize GLEW" << std::endl;
		return -1;
	}
	glViewport(0,0,WIDTH,HEIGHT);
	glEnable(GL_DEPTH_TEST);

	Shader shader("vertex.shader", "fragment.shader");
	Shader lShader("lVertex.shader", "lFragment.shader");	

    GLfloat vertices[] = {
        // Positions          // Normals           // Texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

//BIND OBJECT
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	GLuint lVAO;
	glGenVertexArrays(1, &lVAO);
	glBindVertexArray(lVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	GLuint diffuseMap;
	glGenTextures(1, &diffuseMap);
	int width, height;
	unsigned char* image;
	image=SOIL_load_image("brick.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	shader.Use();
	glUniform1i(glGetUniformLocation(shader.Program, "material.diffuse"), 0);

//LOOP
	while(!glfwWindowShouldClose(window)){
	GLfloat currentTime = glfwGetTime();
	deltaTime = currentTime-lastTime;
	lastTime = currentTime;
	glfwPollEvents();
	do_movement();

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.Use();
	GLint lightAmbientLoc=glGetUniformLocation(shader.Program, "light.ambient");
	GLint lightDiffuseLoc=glGetUniformLocation(shader.Program, "light.diffuse");
	GLint lightSpecularLoc=glGetUniformLocation(shader.Program, "light.specular");  
	glUniform3f(lightAmbientLoc,  0.2f, 0.2f, 0.2f);
	glUniform3f(lightDiffuseLoc,  0.5f, 0.5f, 0.5f);
	glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

	GLint matAmbientLoc=glGetUniformLocation(shader.Program, "material.ambient");
	//GLint matDiffuseLoc=glGetUniformLocation(shader.Program, "material.diffuse");
	GLint matSpecularLoc=glGetUniformLocation(shader.Program, "material.specular");
	GLint matShineLoc=glGetUniformLocation(shader.Program, "material.shininess"); 
	glUniform3f(matAmbientLoc, 0.24f, 0.2f, 0.07f);
	glUniform3f(matSpecularLoc, 0.62f, 0.55f, 0.36f);
	glUniform1f(matShineLoc, 32.0f);

	GLint objectColorLoc=glGetUniformLocation(shader.Program, "objectColor");
	GLint lightColorLoc=glGetUniformLocation(shader.Program, "lightColor");
	GLint lightPosLoc=glGetUniformLocation(shader.Program, "lightPos");
	GLint viewPosLoc=glGetUniformLocation(shader.Program, "viewPos");
	glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

	glm::mat4 view;
	view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 1.5f, 0.1f, 100.0f);
	GLuint modelLoc = glGetUniformLocation(shader.Program, "model");
	GLuint viewLoc = glGetUniformLocation(shader.Program, "view");
	GLuint projLoc = glGetUniformLocation(shader.Program, "projection");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);

	glBindVertexArray(VAO);
	glm::mat4 model;
	model = glm::rotate(model, glm::radians((GLfloat)glfwGetTime()*50.0f), glm::vec3(0.5f, 1.0f, 1.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));	
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	lShader.Use();
	modelLoc=glGetUniformLocation(lShader.Program, "model");
	viewLoc=glGetUniformLocation(lShader.Program, "view");
	projLoc=glGetUniformLocation(lShader.Program, "projection");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	model = glm::mat4();
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.1f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lVAO);
	glDeleteBuffers(1,&VBO);
	glfwTerminate();

	return 0;
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
	if(key==GLFW_KEY_ESCAPE && action==GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
	else if(action==GLFW_PRESS) keys[key] = true;
	else if(action==GLFW_RELEASE) keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	if(firstMouse){lastX=xpos; lastY=ypos; firstMouse=false;}
	GLfloat xOff = xpos-lastX, yOff = ypos-lastY;
	lastX = xpos; lastY = ypos;
	camera.ProcessMouseMovement(xOff, yOff);
}

void scroll_callback(GLFWwindow* window, double xoff, double yoff){
	camera.ProcessMouseScroll(yoff);
}

void do_movement(){
	GLboolean FPS = false;
	if(keys[GLFW_KEY_W]) camera.ProcessKeyboard(FORWARD, deltaTime, FPS);
	if(keys[GLFW_KEY_S]) camera.ProcessKeyboard(BACKWARD, deltaTime, FPS);
	if(keys[GLFW_KEY_A]) camera.ProcessKeyboard(LEFT, deltaTime, FPS);
	if(keys[GLFW_KEY_D]) camera.ProcessKeyboard(RIGHT, deltaTime, FPS);
}
