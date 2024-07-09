#include "pch.h"

// 屏幕宽 高
const GLuint WIDTH = 640, HEIGHT = 480;

int main(void)
{  
    auto window = init::Init(WIDTH, HEIGHT, 3, 3, true);
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
        //// 纹理坐标
        //GLfloat texCoords[] = {
        //    0.0f, 0.0f, // 左下角
        //    1.0f, 0.0f, // 右下角
        //    0.5f, 1.0f // 上中
        //};

        unsigned int vao; // 顶点数组对象(Vertex Array Object, VAO)
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        VertexArray va;
        VertexBuffer vb(vertices, sizeof(vertices)); // VertexBuffer对象

        VertexBufferLayout layout;
        // Cherno 是 2 我此处需要分别为位置和颜色调用Push
        layout.Push<float>(3); // 前三个float是position
        layout.Push<float>(3); // 4-6的float是color
        va.AddBuffer(vb, layout);

        // IndexBuffer对象
        IndexBuffer ib(indices, sizeof(indices) / sizeof(GLuint)); // Cherno第二个参数直接写的6

        Shader basicShader("res/shaders/Basic.shader"); // 读取basic.shader

        basicShader.Bind();

        va.Unbind();
        vb.UnBind();
        ib.UnBind();
        basicShader.Unbind();

        Renderer renderer;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.SetBgColor();
            renderer.Clear();

            basicShader.Bind();

            renderer.Draw(va, ib, basicShader);
           
            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));
            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }
    }
    glfwTerminate();
    return 0;
}