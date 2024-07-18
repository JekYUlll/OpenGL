#ifndef KEYCONTROLL_HPP
#define KEYCONTROLL_HPP
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Camera.h"

namespace kcb
{
	static bool isWireframe = false; // �Ƿ�Ϊ�߿�ģʽ
	static bool keys[1024];

	inline void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) // from LearnOpenGL �̳��к�������key_callback
	{
		if (action == GLFW_PRESS) {
			keys[key] = true;
			// 1. �˳�����
			if (key == GLFW_KEY_ESCAPE) {
				// ����ESC��,����window���ڵ�WindowShouldClose����Ϊtrue
				std::cout << "[press ESC] window closed" << std::endl;
				glfwSetWindowShouldClose(window, GL_TRUE);
			}
			// 2. �л��߿�/���ģʽ
			if (key == GLFW_KEY_P) {
				isWireframe = !isWireframe;
				if (isWireframe) {
					GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));  // �л����߿�ģʽ
					std::cout << "[press P] LINE mode" << std::endl;
				}
				else {
					GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));  // �л������ģʽ
					std::cout << "[press P] FILL mode" << std::endl;
				}
			}
		}
		else if (action == GLFW_RELEASE) {
			keys[key] = false;
		}
	}

	inline void MoveCamera()
	{
		if (kcb::keys[GLFW_KEY_W])
			cameraPos += cameraMove * cameraFront;
		if (kcb::keys[GLFW_KEY_S])
			cameraPos -= cameraMove * cameraFront;
		if (kcb::keys[GLFW_KEY_A])
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraMove;
		if (kcb::keys[GLFW_KEY_D])
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraMove;
	}
}

#endif // !KEYCONTROLL_HPP
