#pragma once
#define KEYCONTROLL_HPP

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// key call back 回调
namespace kcb
{
	static bool isWireframe = false; // 是否为线框模式

	static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mode) // from LearnOpenGL 教程中函数名是key_callback
	{
		if (action == GLFW_PRESS) {
			// 1. 退出窗口
			if (key == GLFW_KEY_ESCAPE) {
				// 按下ESC键,设置window窗口的WindowShouldClose属性为true
				std::cout << "[press ESC] window closed" << std::endl;
				glfwSetWindowShouldClose(window, GL_TRUE);
			}
			// 2. 切换线框/填充模式
			else if (key == GLFW_KEY_P) {
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
		}
	}
}
