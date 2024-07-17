#include "pch.h"

const unsigned int WIDTH = 1792, HEIGHT = 934;
float aspectRatio = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT); // 屏幕比例
float image_scale = 1.0f; // 图像缩放倍率
float ui_scale = 1.4f; // ImGui 缩放

const char* glsl_version = "#version 330";

int main(int argc, char const* argv[])
{  
#ifdef _DEBUG
    std::cout << "[Debug mode]" << std::endl;
#else
    std::cout << "[Release 1.0]" << std::endl;
#endif

    cameraSpeed = 0.05f;


    auto window = init::Init("--- LearnOpenGL", WIDTH, HEIGHT, 3, 3, true);
    if (!window) {
        std::cerr << "Failed to init window!" << std::endl;
        return -1;
    }

    init::SetWindowIcon(window, "res/icon/opengl.png");

    // 设置边框颜色为黑色
    // SetWindowBorderColor(window, RGB(0, 0, 0));

    {   // openGL自身问题：关闭窗口的时候，程序不会完全退出(check error返回一个error无限循环)。因此用作用域框起来(Cherno S13末尾)

        glViewport(0, 0, WIDTH, HEIGHT); // 设置初始视口

        Shader shader("res/shaders/Mix.shader"); // 读取basic.shader
        // 设置用户指针为 Shader 对象
        glfwSetWindowUserPointer(window, &shader);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        // 顶点属性
		//float vertices[] = {
  //          // ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
  //          0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
  //          0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
  //         -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
  //         -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
		//};
        float vertices[] = {
            // ---- 位置 ----       ---- 颜色 ----       - 纹理坐标 -
            // 前面
            -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
            // 后面
            -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
            // 左面
            -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
            // 右面
             0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
             0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f,
            // 顶面
            -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
            // 底面
            -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f
        };

        // index buffer 索引缓冲区
        unsigned int indices[] = {
            // 前面
            0, 1, 2, 2, 3, 0,
            // 后面
            4, 5, 6, 6, 7, 4,
            // 左面
            8, 9, 10, 10, 11, 8,
            // 右面
            12, 13, 14, 14, 15, 12,
            // 上面
            16, 17, 18, 18, 19, 16,
            // 下面
            20, 21, 22, 22, 23, 20
        };

		glm::vec3 cubePositions[] = {
              glm::vec3(0.0f,  0.0f,  0.0f),
              glm::vec3(2.0f,  5.0f, -15.0f),
              glm::vec3(-1.5f, -2.2f, -2.5f),
              glm::vec3(-3.8f, -2.0f, -12.3f),
              glm::vec3(2.4f, -0.4f, -3.5f),
              glm::vec3(-1.7f,  3.0f, -7.5f),
              glm::vec3(1.3f, -2.0f, -2.5f),
              glm::vec3(1.5f,  2.0f, -2.5f),
              glm::vec3(1.5f,  0.2f, -1.5f),
              glm::vec3(-1.3f,  1.0f, -1.5f)
		};

        GLCall(glEnable(GL_DEPTH_TEST)); // 启用深度测试

        GLCall(glEnable(GL_BLEND)); // 启用混合
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(vertices, sizeof(vertices)); // VertexBuffer对象

        VertexBufferLayout layout;
        layout.Push<float>(3); // position
        layout.Push<float>(3); // color
        layout.Push<float>(2); // texture coord

        va.AddBuffer(vb, layout);
        IndexBuffer ib(indices, 6);

        // glm::mat4 projection = glm::ortho(-1.6f, 1.6f, -0.9f, 0.9f, -1.0f, 1.0f); // 正交矩阵
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.01f, 100.0f);
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(image_scale, image_scale, image_scale)); // 缩放矩阵
        // glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3.0f));
        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glm::mat4 model = scale;

        float rotateAngel = 0.0f;
        float rotateSpeed = 1.0f;
        float rotateAxis[3] = {1.0f, 0.5f, 0.5f};

        /*glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
        glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);*/
        
        shader.Bind();

        shader.SetUniformMat4f("view", view);
        shader.SetUniformMat4f("model", model);
        shader.SetUniformMat4f("projection", projection);

        Texture texture1("res/textures/asuka.jpg");
        texture1.Bind(0);
        shader.SetUniform1i("ourTexture1", 0); // glUniform1i可以给纹理采样器分配一个位置值，能够在一个片段着色器中设置多个纹理。一个纹理的位置值通常称为一个纹理单元(Texture Unit)
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
        Gui::getInstance().SetFont("res/font/consola.ttf", ui_scale);
        Gui::getInstance().SetFontGlobalScale(ui_scale);
        ImGuiIO& io = Gui::getInstance().GetIO();

        bool show_demo_window = false;
        ImVec4 clear_color = ImVec4(IMGUI_SKY_BLUE, 1.00f);

#pragma endregion ImGuiInit

        Renderer& renderer = Renderer::getInstance();
        
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();
            renderer.SetBgColor(clear_color);

#pragma region ImGuiLoop

            Gui::getInstance().StartDraw();
            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
            if (show_demo_window)
                ImGui::ShowDemoWindow(&show_demo_window);
            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
            {
                ImGui::Begin("Transform");                          // Create a window called "Hello, world!" and append into it.
                ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
                // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                ImGui::SliderFloat("Scale", &image_scale, 0.02f, 2.75f);

                ImGui::SliderFloat("Rotate Angel", &rotateAngel, -180.0f, 180.0f);
                ImGui::SliderFloat("Rotate Speed", &rotateSpeed, 0.01f, 10.0f);
                ImGui::SliderFloat3("Rotate Axis", rotateAxis, -1.0f, 1.0f);

                if (ImGui::Button("Print"))
                    std::cout << "rotateAngel: " << rotateAngel << "  rotateAxis: " << rotateAxis[0] << ", " << rotateAxis[1] << ", " << rotateAxis[2] << std::endl;
                ImGui::SameLine();
                if (ImGui::Button("Reset")) {
                    image_scale = 1.0f;
                    rotateAngel = 0.0f;
                    rotateSpeed = 1.0f;
                    rotateAxis[0] = 1.0f;
                    rotateAxis[1] = 0.5f;
                    rotateAxis[2] = 0.5f;
                }

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }

#pragma endregion ImGuiLoop

            shader.Bind();

            texture1.Bind(0);
            texture2.Bind(1);

            // rotateSpeed = (float)abs(sin(glfwGetTime())) * 5.0f;

            // 圆
            /*GLfloat radius = 10.0f;
            GLfloat camX = sin(glfwGetTime()) * radius;
            GLfloat camZ = cos(glfwGetTime()) * radius;*/

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(image_scale, image_scale, image_scale));
            model = glm::rotate(model, rotateAngel, glm::vec3(rotateAxis[0], rotateAxis[1], rotateAxis[2]));
            model = glm::rotate(model, (GLfloat)glfwGetTime() * rotateSpeed, glm::vec3(rotateAxis[0], rotateAxis[1], rotateAxis[2]));
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

            shader.SetUniformMat4f("view", view);
            shader.SetUniformMat4f("model", model);
            shader.SetUniformMat4f("projection", projection);
           
            // renderer.DrawElements(va, ib, shader);
            renderer.DrawArrays(va, shader, GL_TRIANGLES, 0, 36);

            gui.Render(window);

            /*for (size_t i = 0; i < 10; i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = glm::radians(20.0f * i);
                model = glm::rotate(model, (float)glfwGetTime() * angle, glm::vec3(1.0f, 0.3f, 0.5f));
                shader.SetUniformMat4f("model", model);

                renderer.DrawArrays(va, shader, GL_TRIANGLES, 0, 36);
            }*/
            GLCall(glfwSwapBuffers(window)); /* Swap front and back buffers */
            GLCall(glfwPollEvents()); /* Poll for and process events */
        }
    }

    Gui::getInstance().CleanUp();
    glfwDestroyWindow(window); 
    glfwTerminate();
    return 0;
}