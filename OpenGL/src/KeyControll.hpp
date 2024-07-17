#ifndef KEYCONTROLL_HPP
#define KEYCONTROLL_HPP
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Camera.h"

namespace kcb
{
	static bool isWireframe = false; // 是否为线框模式

	inline void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) // from LearnOpenGL 教程中函数名是key_callback
	{
		if (action == GLFW_PRESS) {
			// 1. 退出窗口
			if (key == GLFW_KEY_ESCAPE) {
				// 按下ESC键,设置window窗口的WindowShouldClose属性为true
				std::cout << "[press ESC] window closed" << std::endl;
				glfwSetWindowShouldClose(window, GL_TRUE);
			}
			// 2. 切换线框/填充模式
			if (key == GLFW_KEY_P) {
				isWireframe = !isWireframe;
				if (isWireframe) {
					GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));  // 切换到线框模式
					std::cout << "[press P] LINE mode" << std::endl;
				}
				else {
					GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));  // 切换到填充模式
					std::cout << "[press P] FILL mode" << std::endl;
				}
			}
			if (key == GLFW_KEY_W)
				cameraPos += cameraSpeed * cameraFront;
			if (key == GLFW_KEY_S)
				cameraPos -= cameraSpeed * cameraFront;
			if (key == GLFW_KEY_A)
				cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
			if (key == GLFW_KEY_D)
				cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
	}
}

#endif // !KEYCONTROLL_HPP
