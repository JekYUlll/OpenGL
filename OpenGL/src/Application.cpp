#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifndef DEBUG
#define DEBUG
#endif // !DEBUG

#include "Renderer.h"
#include "Debug.hpp"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "KeyControll.hpp"

// 屏幕宽 高
const GLuint WIDTH = 640, HEIGHT = 480;

// 自己写的
static void SetVersion(const unsigned int& num_1, const unsigned int& num_2, const bool& isCore)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, num_1); // 主版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, num_2); // 次版本
    if(isCore)
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 配置设为核心配置文件(core)
    else
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // 兼容配置文件(compat)
}

// 打印初始信息 (自己改写)
static void PrintInitInfo() {
    // 打印版本号
    std::cout << "GLEW version: " << glGetString(GL_VERSION) << std::endl;
    // 打印分辨率
    std::cout << "resolution: " << WIDTH << " * " << HEIGHT << std::endl;
    // 打印顶点属性上限
    GLint nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "maximum nr of vertex attributes supported: " << nrAttributes << std::endl << std::endl;
}

int main(void)
{
#pragma region Init

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    SetVersion(3, 3, true);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Cherno OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // from LearnOpenGL 通过GLFW注册我们的函数至合适的回调 (我以为是依次添加，后来发现实际上只能绑定一个。因此只需要一个CallBack函数)
    GLCall(glfwSetKeyCallback(window, kcb::KeyCallBack));

    glfwSwapInterval(1); // 设置垂直同步，帧数限定在60

    // GLEW
    if (glewInit() != GLEW_OK)
        std::cout << "GLEW init Error!" << std::endl;
    
    PrintInitInfo();

#pragma endregion Init
    {   // openGL自身问题：关闭窗口的时候，程序不会完全退出(check error返回一个error无限循环)。因此用作用域框起来(Cherno S13末尾)
        // 顶点位置的浮点数组(顶点不光包含位置，还有法线等信息)

        // 顶点属性
		GLfloat vertices[] = {
			// 位置                // 颜色
	        0.5f,  -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,   // 右下
           -0.5f,  -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,   // 左下
	        0.0f,   0.5f,  0.0f,  0.0f,  0.0f,  1.0f    // 顶部
		};

        // index buffer 索引缓冲区
        GLuint indices[] = {
            0, 1, 2
        };
        // 如果画矩形，就是 0, 1, 2, 2, 3, 0

        unsigned int vao; // 顶点数组对象(Vertex Array Object, VAO)
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        VertexArray va;
        VertexBuffer vb(vertices, sizeof(vertices)); // VertexBuffer对象

        VertexBufferLayout layout;
        layout.Push<float>(3); // Cherno 是 2 我此处需要分别为位置和颜色调用Push
        layout.Push<float>(3); 
        va.AddBuffer(vb, layout);

        // IndexBuffer对象
        IndexBuffer ib(indices, sizeof(indices) / sizeof(GLuint)); // Cherno第二个参数直接写的6

        std::string basicShaderPath = "res/shaders/Basic.shader"; // basic的相对路径
        std::string basicAbsolutePath = "F:/Program/_VS/OpenGL/OpenGL/res/shaders/Basic.shader"; // basic的绝对路径
        std::string vertexPath = "res/shaders/3ct_vertex.shader";
        std::string fragmentPath = "res/shaders/3ct_fragment.shader";

        //Shader basicShader(vertexPath.c_str(), fragmentPath.c_str()); // 两个文件分开读取

        Shader basicShader(basicShaderPath); // 读取basic.shader，报错

        basicShader.Bind();

        va.Unbind();
        vb.UnBind();
        ib.UnBind();
        basicShader.Unbind();

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            GLCall(glClearColor(0.02f, 0.06f, 0.08f, 1.0f)); // 背景颜色
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            basicShader.Bind();

            va.Bind();
            ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr)); // 绘制
           
            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));
            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }
    }
    glfwTerminate();
    return 0;
}