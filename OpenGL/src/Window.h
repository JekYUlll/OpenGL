#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_EXPOSE_NATIVE_WIN32

#include <iostream>
// #include <glad/glad.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

// �ص��������ڸı䴰�ڷֱ��ʵ�ʱ�򱣳�ͼƬ����Ȳ���
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// ͼƬ��������䴰��
void AdaptToWindow(int& width, int& height, int winW, int WinH);
// �Ѵ��ڱ߿�����Ϊ��ɫ��û��Ч
// void SetWindowBorderColor(GLFWwindow* window, COLORREF color);

#endif // !WINDOW_H

