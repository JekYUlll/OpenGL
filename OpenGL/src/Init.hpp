#ifndef INIT_HPP
#define INIT_HPP

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image/stb_image.h"

#include "Console.h"
#include "Debug.h"
#include "Controll.h"
#include "config.h"
#include "Gui.h"
#include "Renderer.h"

namespace init {
    // ���ð汾
    static void SetOpenGLVersion(const unsigned int& versionNum_1, const unsigned int& versionNum_2, const bool& isCore)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionNum_1); // ���汾
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionNum_2); // �ΰ汾
        if (isCore)
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // ������Ϊ���������ļ�(core)
        else
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // ���������ļ�(compat)
    }
    // ��ӡ��ʼ��Ϣ
    static void PrintInitInfo(const unsigned int& width, const unsigned int& height) {
#ifdef _DEBUG
        std::cout << "[Debug mode]" << std::endl;
#else
        std::cout << "[Release 1.0]" << std::endl;
#endif
        if (Renderer::getInstance().GetRenderMode() == RenderMode::MODEL) {
            std::cout << "[Render Mode: MODEL]" << std::endl;
        }
        else if (Renderer::getInstance().GetRenderMode() == RenderMode::CUBE)
        {
            std::cout << "[Render Mode: CUBE]" << std::endl;
        }
        SET_CONSOLE_TEXT_COLOR(FOREGROUND_GREEN);
        std::cout << "  ___                    ____ _    \n"
                  << " / _ \\ _ __   ___ _ __  / ___| |   \n"
                  << "| | | | '_ \\ / _ \\ '_ \\| |  _| |   \n"
                  << "| |_| | |_) |  __/ | | | |_| | |   \n"
                  << " \\___/| .__/ \\___|_| |_|\\____|_|   \n"
                  << "      |_|                          \n" << std::endl; // pyfiglet
        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "ImGui version: " << IMGUI_VERSION << std::endl;
        // ��ӡ�ֱ���
        std::cout << "Resolution: " << width << " * " << height << std::endl;
        int nrAttributes = 0; // ������������
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
        std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
        SET_CONSOLE_TEXT_COLOR(FOREGROUND_RED);
        std::cout << "  <======>\n"  << std::endl;
        SET_CONSOLE_TEXT_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    // ���ô���ͼ��
    static void SetWindowIcon(GLFWwindow* window, const std::string& iconPath) {
        int width, height, channels;
        unsigned char* pixels = stbi_load(iconPath.c_str(), &width, &height, &channels, 4);
        if (pixels) {
            GLFWimage icon{};
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
    /// <summary>
    /// ��ʼ��GLEW��GLFW��ImGui����������
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
            return nullptr;
        }
        init::SetOpenGLVersion(versionNum_1, versionNum_2, isCore);
        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(width, height, windowName.c_str(), NULL, NULL);
        if (!window) {
            glfwTerminate();
            std::cout << "Failed to create window!" << std::endl;
            return nullptr;
        }
        SetWindowIcon(window, "res/icon/opengl.png"); // ����ͼ��
        glfwMakeContextCurrent(window); /* Make the window's context current */
#ifdef VERTICAL_SYN
        GLCall(glfwSwapInterval(1)); // ���ô�ֱͬ��
#endif // VERTICAL_SYN
        GLCall(glfwSetKeyCallback(window, ctrl::key_callback)); // ���ð����ص�
        GLCall(glfwSetCursorPosCallback(window, ctrl::mouse_callback)); // �������ص�
        GLCall(glfwSetScrollCallback(window, ctrl::scroll_callback)); // ע������ص�����
        // GLCall(glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED)); // ��������ָ��
        // ��ʼ�� GLEW
        if (glewInit() != GLEW_OK) {
            std::cout << "GLEW init Error!" << std::endl;
            return nullptr;
        }
        PrintInitInfo(width, height);
        return window;
    }
}

#endif // !INIT_HPP
