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

// ��Ļ�� ��
const GLuint WIDTH = 640, HEIGHT = 480;

// �Լ�д��
static void SetVersion(const unsigned int& num_1, const unsigned int& num_2, const bool& isCore)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, num_1); // ���汾
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, num_2); // �ΰ汾
    if(isCore)
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // ������Ϊ���������ļ�(core)
    else
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // ���������ļ�(compat)
}

// ��ӡ��ʼ��Ϣ (�Լ���д)
static void PrintInitInfo() {
    // ��ӡ�汾��
    std::cout << "GLEW version: " << glGetString(GL_VERSION) << std::endl;
    // ��ӡ�ֱ���
    std::cout << "resolution: " << WIDTH << " * " << HEIGHT << std::endl;
    // ��ӡ������������
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

    // from LearnOpenGL ͨ��GLFWע�����ǵĺ��������ʵĻص� (����Ϊ��������ӣ���������ʵ����ֻ�ܰ�һ�������ֻ��Ҫһ��CallBack����)
    GLCall(glfwSetKeyCallback(window, kcb::KeyCallBack));

    glfwSwapInterval(1); // ���ô�ֱͬ����֡���޶���60

    // GLEW
    if (glewInit() != GLEW_OK)
        std::cout << "GLEW init Error!" << std::endl;
    
    PrintInitInfo();

#pragma endregion Init
    {   // openGL�������⣺�رմ��ڵ�ʱ�򣬳��򲻻���ȫ�˳�(check error����һ��error����ѭ��)������������������(Cherno S13ĩβ)
        // ����λ�õĸ�������(���㲻�����λ�ã����з��ߵ���Ϣ)

        // ��������
		GLfloat vertices[] = {
			// λ��                // ��ɫ
	        0.5f,  -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,   // ����
           -0.5f,  -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,   // ����
	        0.0f,   0.5f,  0.0f,  0.0f,  0.0f,  1.0f    // ����
		};

        // index buffer ����������
        GLuint indices[] = {
            0, 1, 2
        };
        // ��������Σ����� 0, 1, 2, 2, 3, 0

        unsigned int vao; // �����������(Vertex Array Object, VAO)
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        VertexArray va;
        VertexBuffer vb(vertices, sizeof(vertices)); // VertexBuffer����

        VertexBufferLayout layout;
        layout.Push<float>(3); // Cherno �� 2 �Ҵ˴���Ҫ�ֱ�Ϊλ�ú���ɫ����Push
        layout.Push<float>(3); 
        va.AddBuffer(vb, layout);

        // IndexBuffer����
        IndexBuffer ib(indices, sizeof(indices) / sizeof(GLuint)); // Cherno�ڶ�������ֱ��д��6

        std::string basicShaderPath = "res/shaders/Basic.shader"; // basic�����·��
        std::string basicAbsolutePath = "F:/Program/_VS/OpenGL/OpenGL/res/shaders/Basic.shader"; // basic�ľ���·��
        std::string vertexPath = "res/shaders/3ct_vertex.shader";
        std::string fragmentPath = "res/shaders/3ct_fragment.shader";

        //Shader basicShader(vertexPath.c_str(), fragmentPath.c_str()); // �����ļ��ֿ���ȡ

        Shader basicShader(basicShaderPath); // ��ȡbasic.shader������

        basicShader.Bind();

        va.Unbind();
        vb.UnBind();
        ib.UnBind();
        basicShader.Unbind();

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            GLCall(glClearColor(0.02f, 0.06f, 0.08f, 1.0f)); // ������ɫ
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            basicShader.Bind();

            va.Bind();
            ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr)); // ����
           
            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));
            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }
    }
    glfwTerminate();
    return 0;
}