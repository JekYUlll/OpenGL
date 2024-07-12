#include "pch.h"

// ��Ļ�� ��
const GLuint WIDTH = 1920, HEIGHT = 1080;
// ���ű���
const GLfloat SCALE = 1.0f;
// ImGui ����
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
        // Cherno �� 2 �Ҵ˴���Ҫ�ֱ�Ϊλ�ú���ɫ����Push
        layout.Push<float>(3); // ǰ����float��position
        layout.Push<float>(3); // color
        layout.Push<float>(2); // ��������

        va.AddBuffer(vb, layout);
        // IndexBuffer����
        IndexBuffer ib(indices, 6);

        glm::mat4 proj = glm::ortho(-1.6f, 1.6f, -0.9f, 0.9f, -1.0f, 1.0f); // ��������
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(SCALE, SCALE, 1.0f)); // ���ž���
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glm::mat4 model = scale;
        glm::mat4 mvp = proj * view * model;

        Shader shader("res/shaders/Basic.shader"); // ��ȡbasic.shader
        shader.Bind();

        shader.SetUniformMat4f("u_MVP", mvp);

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

        
        // ���������С
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