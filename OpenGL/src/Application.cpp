#include "pch.h"

// ��Ļ�� ��
const GLuint WIDTH = 640, HEIGHT = 480;

int main(void)
{  
    auto window = init::Init(WIDTH, HEIGHT, 3, 3, true);
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
        //// ��������
        //GLfloat texCoords[] = {
        //    0.0f, 0.0f, // ���½�
        //    1.0f, 0.0f, // ���½�
        //    0.5f, 1.0f // ����
        //};

        unsigned int vao; // �����������(Vertex Array Object, VAO)
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        VertexArray va;
        VertexBuffer vb(vertices, sizeof(vertices)); // VertexBuffer����

        VertexBufferLayout layout;
        // Cherno �� 2 �Ҵ˴���Ҫ�ֱ�Ϊλ�ú���ɫ����Push
        layout.Push<float>(3); // ǰ����float��position
        layout.Push<float>(3); // 4-6��float��color
        va.AddBuffer(vb, layout);

        // IndexBuffer����
        IndexBuffer ib(indices, sizeof(indices) / sizeof(GLuint)); // Cherno�ڶ�������ֱ��д��6

        Shader basicShader("res/shaders/Basic.shader"); // ��ȡbasic.shader

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