#ifndef INIT_HPP
#define INIT_HPP
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Debug.h"
#include "KeyControll.hpp"
#include "vender/stb_image/stb_image.h"

// ȡ��GLCall�����ٳ�ʼ��
#ifdef DEBUG
#undef DEBUG
#endif // DEBUG

namespace init {
    // ���ð汾
    void SetVersion(const unsigned int& versionNum_1, const unsigned int& versionNum_2, const bool& isCore)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionNum_1); // ���汾
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionNum_2); // �ΰ汾
        if (isCore)
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // ������Ϊ���������ļ�(core)
        else
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // ���������ļ�(compat)
    }

    // ��ӡ��ʼ��Ϣ
    void PrintInitInfo(const unsigned int& width, const unsigned int& height) {
        // ��ӡ�汾��
        std::cout << "GLEW version: " << glGetString(GL_VERSION) << std::endl;
        // ��ӡ�ֱ���
        std::cout << "Resolution: " << width << " * " << height << std::endl;
        // ��ӡ������������
        GLint nrAttributes = 0;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
        std::cout << "maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
        std::cout << "  <=====================================>" << std::endl << std::endl;
    }
    /// <summary>
    /// ��ʼ��GLEW��GLFW����������
    /// </summary>
    /// <param name="windowName">���ڱ���</param>
    /// <param name="width">���ڿ��</param>
    /// <param name="height">���ڸ߶�</param>
    /// <param name="versionNum_1">���汾��</param>
    /// <param name="versionNum_2">�ΰ汾��</param>
    /// <param name="isCore">�Ƿ�ΪOpenGL core</param>
    /// <returns>����ָ��</returns>
    GLFWwindow* Init(const std::string& windowName, const unsigned int& width, const unsigned int& height, const unsigned int& versionNum_1, const unsigned int& versionNum_2, const bool& isCore)
    {
        GLFWwindow* window;
        /* Initialize the library */
        if (!glfwInit()) {
            std::cout << "Failed to init GLFW!" << std::endl;
            std::abort(); // �÷����ܲ�̫��
        }
        init::SetVersion(versionNum_1, versionNum_2, isCore);
        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(width, height, windowName.c_str(), NULL, NULL);
        if (!window) {
            glfwTerminate();
            std::cout << "Failed to create window!" << std::endl;
            std::abort();
        }
        /* Make the window's context current */
        glfwMakeContextCurrent(window);
        // ���ð����ص�
        glfwSetKeyCallback(window, kcb::KeyCallBack);
        // ���ô�ֱͬ����֡���޶���60
        glfwSwapInterval(1);
        // ��ʼ�� GLEW
        if (glewInit() != GLEW_OK) {
            std::cout << "GLEW init Error!" << std::endl;
            std::abort();
        }
        PrintInitInfo(width, height);
        return window;
    }
    // ����ͼ��
    void SetWindowIcon(GLFWwindow* window, const std::string& iconPath) {
        int width, height, channels;
        unsigned char* pixels = stbi_load(iconPath.c_str(), &width, &height, &channels, 4);
        if (pixels) {
            GLFWimage icon;
            icon.width = width;
            icon.height = height;
            icon.pixels = pixels;
            glfwSetWindowIcon(window, 1, &icon);
            stbi_image_free(pixels);
        }
        else {
            std::cerr << "Failed to load icon image: " << iconPath << std::endl;
        }
    }
}

#endif // !INIT_HPP
