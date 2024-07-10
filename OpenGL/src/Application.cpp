#include "pch.h"

// 屏幕宽 高
const GLuint WIDTH = 704, HEIGHT = 755;

int main(void)
{  
    auto window = init::Init("LearnOpenGL", WIDTH, HEIGHT, 3, 3, true);
    init::SetWindowIcon(window, "res/icon/chris.png");

    {   // openGL自身问题：关闭窗口的时候，程序不会完全退出(check error返回一个error无限循环)。因此用作用域框起来(Cherno S13末尾)
        // 顶点位置的浮点数组(顶点不光包含位置，还有法线等信息)

        // 顶点属性
		GLfloat vertices[] = {
            // ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
           -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
           -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
		};
        // index buffer 索引缓冲区
        GLuint indices[] = {
            0, 1, 3, // 第一个三角形
            1, 2, 3  // 第二个三角形
        };

        GLCall(glEnable(GL_BLEND)); // 启用混合
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(vertices, sizeof(vertices)); // VertexBuffer对象

        VertexBufferLayout layout;
        // Cherno 是 2 我此处需要分别为位置和颜色调用Push
        layout.Push<float>(3); // 前三个float是position
        layout.Push<float>(3); // color
        layout.Push<float>(2); // 纹理坐标

        va.AddBuffer(vb, layout);
        // IndexBuffer对象
        IndexBuffer ib(indices, 6);
        // IndexBuffer ib(indices, sizeof(indices) / sizeof(GLuint)); // Cherno第二个参数直接写的6

        Shader shader("res/shaders/Basic.shader"); // 读取basic.shader
        shader.Bind();

        Texture texture1("res/textures/pathetic.png");
        texture1.Bind(0);
        shader.SetUniform1i("ourTexture1", 0); // glUniform1i可以给纹理采样器分配一个位置值，能够在一个片段着色器中设置多个纹理。一个纹理的位置值通常称为一个纹理单元(Texture Unit)
        
        Texture texture2("res/textures/awesomeface.png");
        texture2.Bind(1);
        shader.SetUniform1i("ourTexture2", 1);

        va.Unbind();
        vb.UnBind();
        ib.UnBind();
        shader.Unbind();

        Renderer renderer;
        renderer.SetBgColor(ARMY_GREEN);
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            shader.Bind();

            texture1.Bind(0); // 少了这两句，排查一下午
            texture2.Bind(1);

            renderer.Draw(va, ib, shader);
            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));
            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }
    }
    glfwDestroyWindow(window); // 好像不需要
    glfwTerminate();
    return 0;
}