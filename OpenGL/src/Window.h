#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>

// #include <glad/glad.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

// �ص��������ڸı䴰�ڷֱ��ʵ�ʱ�򱣳�ͼƬ����Ȳ���
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// ͼƬ��������䴰��
void AdaptToWindow(int& width, int& height, int winW, int WinH);

#endif // !WINDOW_H

