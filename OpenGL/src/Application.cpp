#include "pch.h"

// ��Ļ�� ��
const GLuint WIDTH = 704, HEIGHT = 755;

int main(void)
{  
    auto window = init::Init("LearnOpenGL", WIDTH, HEIGHT, 3, 3, true);
    init::SetWindowIcon(window, "res/icon/chris.png");

    {   // openGL�������⣺�رմ��ڵ�ʱ�򣬳��򲻻���ȫ�˳�(check error����һ��error����ѭ��)������������������(Cherno S13ĩβ)
        // ����λ�õĸ�������(���㲻�����λ�ã����з��ߵ���Ϣ)

        // ��������
		GLfloat vertices[] = {
            // ---- λ�� ----       ---- ��ɫ ----     - �������� -
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
           -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
           -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
		};
        // index buffer ����������
        GLuint indices[] = {
            0, 1, 3, // ��һ��������
            1, 2, 3  // �ڶ���������
        };

        GLCall(glEnable(GL_BLEND)); // ���û��
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(vertices, sizeof(vertices)); // VertexBuffer����

        VertexBufferLayout layout;
        // Cherno �� 2 �Ҵ˴���Ҫ�ֱ�Ϊλ�ú���ɫ����Push
        layout.Push<float>(3); // ǰ����float��position
        layout.Push<float>(3); // color
        layout.Push<float>(2); // ��������

        va.AddBuffer(vb, layout);
        // IndexBuffer����
        IndexBuffer ib(indices, 6);
        // IndexBuffer ib(indices, sizeof(indices) / sizeof(GLuint)); // Cherno�ڶ�������ֱ��д��6

        Shader shader("res/shaders/Basic.shader"); // ��ȡbasic.shader
        shader.Bind();

        Texture texture1("res/textures/pathetic.png");
        texture1.Bind(0);
        shader.SetUniform1i("ourTexture1", 0); // glUniform1i���Ը��������������һ��λ��ֵ���ܹ���һ��Ƭ����ɫ�������ö������һ�������λ��ֵͨ����Ϊһ������Ԫ(Texture Unit)
        
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

            texture1.Bind(0); // ���������䣬�Ų�һ����
            texture2.Bind(1);

            renderer.Draw(va, ib, shader);
            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));
            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }
    }
    glfwDestroyWindow(window); // ������Ҫ
    glfwTerminate();
    return 0;
}