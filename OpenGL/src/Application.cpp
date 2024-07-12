#include "pch.h"

// 屏幕宽 高
const GLuint WIDTH = 1920, HEIGHT = 1080;
// 缩放倍率
const GLfloat SCALE = 1.0f;
// ImGui 缩放
float highDPIscaleFactor = 2.0f;

static float f = 0.0f;
static int counter = 0;

const char* glsl_version = "#version 330";


int main(void)
{  
#ifdef _DEBUG
    std::cout << "[Debug mode]" << std::endl;
#else
    std::cout << "[Release 1.0]" << std::endl;
#endif

    auto window = init::Init("LearnOpenGL", WIDTH, HEIGHT, 3, 3, true);
    init::SetWindowIcon(window, "res/icon/opengl.png");

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
            0, 1, 3, // 第一个三角形//
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

        glm::mat4 proj = glm::ortho(-1.6f, 1.6f, -0.9f, 0.9f, -1.0f, 1.0f); // 正交矩阵
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(SCALE, SCALE, 1.0f)); // 缩放矩阵
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glm::mat4 model = scale;
        glm::mat4 mvp = proj * view * model;

        Shader shader("res/shaders/Basic.shader"); // 读取basic.shader
        shader.Bind();

        shader.SetUniformMat4f("u_MVP", mvp);

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

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        io.Fonts->Clear(); // Clear previous fonts
        static const ImWchar ranges[] = { 0x0020, 0x00FF, 0x4e00, 0x9FAF, 0 }; // Basic Latin + Chinese characters
        ImFont* font = io.Fonts->AddFontFromFileTTF("res/font/consola.ttf", 8.0f * highDPIscaleFactor, NULL, ranges);
        if (!font)
        {
            std::cerr << "Failed to load font!" << std::endl;
            return -1;
        }

        
        // 设置字体大小
        io.FontGlobalScale = highDPIscaleFactor;
       
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        bool show_demo_window = true;
        bool show_another_window = false;
        // ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        ImVec4 clear_color = ImVec4(ARMY_GREEN, 1.00f);

#pragma endregion ImGuiInit

        Renderer& renderer = Renderer::getInstance();

        // renderer.SetBgColor(ARMY_GREEN);
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();
            glfwPollEvents();

#pragma region ImGuiWhile
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
            if (show_demo_window)
                ImGui::ShowDemoWindow(&show_demo_window);

            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
            {
                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                ImGui::Text("This is a text.");               // Display some text (you can use a format strings too)
                ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
                ImGui::Checkbox("Another Window", &show_another_window);

                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

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
            // Rendering
            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#pragma endregion ImGuiWhile

            shader.Bind();

            texture1.Bind(0);
            texture2.Bind(1);

            renderer.Draw(va, ib, shader);
            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));
            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }
    }
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window); 
    glfwTerminate();
    return 0;
}