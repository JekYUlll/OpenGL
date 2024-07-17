#ifndef INIT_HPP
#define INIT_HPP
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Debug.h"
#include "KeyControll.hpp"
#include "stb_image/stb_image.h"
#include "Gui.h"

// 取消GLCall，加速初始化
#ifdef DEBUG
#undef DEBUG
#endif // DEBUG

namespace init {
    // 设置版本
    void SetOpenGLVersion(const unsigned int& versionNum_1, const unsigned int& versionNum_2, const bool& isCore)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionNum_1); // 主版本
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionNum_2); // 次版本
        if (isCore)
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 配置设为核心配置文件(core)
        else
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // 兼容配置文件(compat)
    }

    // 打印初始信息
    void PrintInitInfo(const unsigned int& width, const unsigned int& height) {
        // 打印版本号
        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
        // 打印分辨率
        std::cout << "Resolution: " << width << " * " << height << std::endl;
        // 打印顶点属性上限
        GLint nrAttributes = 0;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
        std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
        std::cout << "  <=====================================>" << std::endl << std::endl;
    }
    /// <summary>
    /// 初始化GLEW和GLFW，创建窗口
    /// </summary>
    /// <param name="windowName">窗口标题</param>
    /// <param name="width">窗口宽度</param>
    /// <param name="height">窗口高度</param>
    /// <param name="versionNum_1">主版本号</param>
    /// <param name="versionNum_2">次版本号</param>
    /// <param name="isCore">是否为OpenGL core</param>
    /// <returns>窗口指针</returns>
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
        /* Make the window's context current */
        glfwMakeContextCurrent(window);
        // 设置按键回调
        glfwSetKeyCallback(window, kcb::key_callback);
        // 设置垂直同步，帧数限定在60
        glfwSwapInterval(1);
        // 初始化 GLEW
        if (glewInit() != GLEW_OK) {
            std::cout << "GLEW init Error!" << std::endl;
            return nullptr;
        }
        PrintInitInfo(width, height);
        return window;
    }
    // 设置窗口图标
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
