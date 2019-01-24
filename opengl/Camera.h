#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>
using namespace std;


GLdouble currenttime = 0.0f;
GLdouble lasttime = 0.0f;
GLdouble deltatime = 0.0f;


class Camera {

private:
	glm::vec3 cameraPosition;
	glm::vec3 viewPoint;
	glm::vec3 upVector;

	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;

	GLfloat pitch;
	GLfloat yaw;

	GLfloat movementSensitive;
	GLfloat movementSpeed;


	void Update() {

		
		Front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		Front.y = sin(glm::radians(pitch));
		Front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		Front = glm::normalize(Front);

		Right = glm::normalize(glm::cross(Front, upVector)); 
		Up = glm::normalize(glm::cross(Right, Front));
	}
	


public:

	GLfloat fov;//����
	GLboolean key_status[1024];//���̱�־����


	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 view = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
		:Front(glm::vec3(view - position)), pitch(0.0f), yaw(-90.0f),movementSensitive(0.1f), movementSpeed(2.5f), fov(45.0f)
	{
	
		cameraPosition = position;
		viewPoint = view;
		upVector = up;

		Update();
	}



	//�����ƶ��ı������λ��  ���ı�۲�ķ���
	void Key_Movement() {

		currenttime = glfwGetTime();
		deltatime = currenttime - lasttime;
		lasttime = currenttime;

		GLfloat vary = movementSpeed * deltatime;

		if (key_status[GLFW_KEY_W] ) //GLFW_KEY_W
			cameraPosition += Front * vary;
		if (key_status[GLFW_KEY_S] )//GLFW_KEY_S
			cameraPosition -= Front * vary;
		if (key_status[GLFW_KEY_A] )//GLFW_KEY_A
			cameraPosition -= Right * vary;
		if (key_status[GLFW_KEY_D])//GLFW_KEY_D
			cameraPosition += Right * vary;


	}

	//����ƶ��ı�۲�Ƕȣ�Ҳ���Ǹı��˹۲�ķ���
	void Mouse_Movement(GLdouble Xoffset, GLdouble Yoffset, GLboolean test = true) {

		Xoffset *= movementSensitive;//ˮƽƫ��Ӱ��yaw
		Yoffset *= movementSensitive;//��ֱƫ��Ӱ��pitch
		pitch += Yoffset;
		yaw += Xoffset;
	

		if (test) {

			if (pitch < -89.0f)
				pitch = -89.0f;//������
			if (pitch > 89.0f)
				pitch = 89.0f;//������
		}

		Update();
	}


	//���ֽ��зŴ���С �ı����� 45.0f��Ĭ��ֵ
	void Cursor_Scoll(GLdouble Yoffset) {

		if (fov >= 1.0f && fov <= 45.0f)
			fov -= Yoffset;
		if (fov <= 1.0f)
			fov = 1.0f;
		if (fov >= 45.0f)
			fov = 45.0f;
	}

	glm::mat4 GetViewMatrix() {
		
		//cout <<"FRONT:  "<< Front.x << " " << Front.y << " " << Front.z << " " << endl;
		return glm::lookAt(cameraPosition,Front+cameraPosition, Up);
	}


};
