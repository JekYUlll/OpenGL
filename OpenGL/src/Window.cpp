#include "Window.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    std::cout << "Framebuffer size changed: " << width << "x" << height << std::endl;
    glViewport(0, 0, width, height);

    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    Shader* shader = static_cast<Shader*>(glfwGetWindowUserPointer(window));
    shader->Bind();
    shader->SetUniformMat4f("projection", projection);
}

void AdaptToWindow(int& width, int& height, int winW, int WinH)
{
	double  wRatio = winW * 1.0f / width;
	double  hRatio = WinH * 1.0f / height;
	if (wRatio < hRatio) {
		width = winW;
		height = (int)(wRatio * height);
	}
	else {
		height = WinH;
		width = (int)(hRatio * width);
	}
}