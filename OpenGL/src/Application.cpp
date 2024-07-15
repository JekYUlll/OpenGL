#include "pch.h"

const GLuint WIDTH = 1920, HEIGHT = 1080;
static float aspectRatio = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT);
// ���ű���
GLfloat SCALE = 1.0f;
// ImGui ����
float highDPIscaleFactor = 1.5f;
const char* glsl_version = "#version 330";


int main(int argc, char const* argv[])
{  
#ifdef _DEBUG
    std::cout << "[Debug mode]" << std::endl;
#else
    std::cout << "[Release 1.0]" << std::endl;
#endif

    auto window = init::Init("LearnOpenGL", WIDTH, HEIGHT, 3, 3, true);
    if (!window) {
        std::cerr << "Failed to init window!" << std::endl;
        return -1;
    }
    init::SetWindowIcon(window, "res/icon/opengl.png");

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
            0, 1, 3, // ��һ��������//
            1, 2, 3  // �ڶ���������
        };

        GLCall(glEnable(GL_BLEND)); // ���û��
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(vertices, sizeof(vertices)); // VertexBuffer����

        VertexBufferLayout layout;
        layout.Push<float>(3); // position
        layout.Push<float>(3); // color
        layout.Push<float>(2); // texture coord

        va.AddBuffer(vb, layout);
        IndexBuffer ib(indices, 6);
        
         // ��ʼ
         glm::mat4 projection = glm::ortho(-1.6f, 1.6f, -0.9f, 0.9f, -1.0f, 1.0f); // ��������
         // projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
         glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(SCALE, SCALE, SCALE)); // ���ž���
         glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
         glm::mat4 model = scale;
         float rotateAngel = 0.0f;
         float rotateAxis[3] = {1.0f, 0.0f, 0.0f};
         
        


        //glm::mat4 model;
        //glm::mat4 view;
        //glm::mat4 projection;
        //glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(SCALE, SCALE, SCALE)); // ���ž���
        //projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
        //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        //model = glm::rotate(model, -55.0f, glm::vec3(1.0f, 0.0f, 0.0f));

        Shader shader("res/shaders/Basic.shader"); // ��ȡbasic.shader
        shader.Bind();

        shader.SetUniformMat4f("view", view);
        shader.SetUniformMat4f("model", model);
        shader.SetUniformMat4f("projection", projection);

        Texture texture1("res/textures/asuka.jpg");
        texture1.Bind(0);
        shader.SetUniform1i("ourTexture1", 0); // glUniform1i���Ը��������������һ��λ��ֵ���ܹ���һ��Ƭ����ɫ�������ö������һ�������λ��ֵͨ����Ϊһ������Ԫ(Texture Unit)
        Texture texture2("res/textures/calm.png");
        texture2.Bind(1);
        shader.SetUniform1i("ourTexture2", 1);

        va.Unbind();
        vb.UnBind();
        ib.UnBind();
        shader.Unbind();

#pragma region ImGuiInit

        Gui& gui = Gui::getInstance();
        Gui::getInstance().Init(window, true, glsl_version);
        Gui::getInstance().SetFont("res/font/consola.ttf", highDPIscaleFactor);
        Gui::getInstance().SetFontGlobalScale(highDPIscaleFactor);
        ImGuiIO& io = Gui::getInstance().GetIO();

        bool show_demo_window = true;
        bool show_another_window = false;
        // ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        ImVec4 clear_color = ImVec4(PERPLISH_BLUE, 1.00f);

#pragma endregion ImGuiInit

        Renderer& renderer = Renderer::getInstance();

        // renderer.SetBgColor(ARMY_GREEN);
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            
            
            renderer.Clear();

#pragma region ImGuiWhile

            Gui::getInstance().StartDraw();

            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
            if (show_demo_window)
                ImGui::ShowDemoWindow(&show_demo_window);

            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
            {
                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                ImGui::Text("This is a text.");               // Display some text (you can use a format strings too)
                ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
                ImGui::Checkbox("Another Window", &show_another_window);

                            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                ImGui::SliderFloat("SCALE", &SCALE, 0.0f, 100.0f);

                ImGui::SliderFloat("rotateAngel", &rotateAngel, -180.0f, 180.0f);
                ImGui::SliderFloat3("Rotate Axis", rotateAxis, -1.0f, 1.0f);

                if (ImGui::Button("Print rotate"))
                    std::cout << "rotateAngel: " << rotateAngel << "  rotateAxis: " << rotateAxis[0] << ", " << rotateAxis[1] << ", " << rotateAxis[2] << std::endl;
                // ImGui::SameLine();

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }
            // 3. Show another simple window.
            if (show_another_window)
            {
                ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                ImGui::Text("Hello from another window!");
                if (ImGui::Button("Close Me"))
                    show_another_window = false;
                ImGui::End();
            }
            
            gui.Render(window, clear_color);

#pragma endregion ImGuiWhile

            shader.Bind();

            texture1.Bind(0);
            texture2.Bind(1);

            glm::mat4 model = glm::mat4(1.0f); // ���ɾ����һ�У�����ʵ��һֱ��ת��Ч��
            model = glm::scale(model, glm::vec3(SCALE, SCALE, SCALE));
            model = glm::rotate(model, rotateAngel, glm::vec3(rotateAxis[0], rotateAxis[1], rotateAxis[2]));
            shader.SetUniformMat4f("model", model);
           
            renderer.Draw(va, ib, shader);


            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));
            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }
    }
    
    Gui::getInstance().CleanUp();

    glfwDestroyWindow(window); 
    glfwTerminate();
    return 0;
}