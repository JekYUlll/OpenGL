#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define DEBUG

#include "Renderer.h"
#include "Debug.hpp"
#include "KeyControll.hpp"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

// 屏幕宽 高
const GLuint WIDTH = 640, HEIGHT = 480;

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

struct ShaderProgramSources
{
    std::string VertexSource;
    std::string FragmentSource;
};

// 读取并切分shader文件
static ShaderProgramSources ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);
	// 正在读取的shader类型
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (std::getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				// set mode to vertex
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				// set mode to fragment
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(GLint)type] << line << '\n'; 
		}
	}

	return { ss[0].str(),ss[1].str() };
}

static GLuint CompileShader(GLuint type, const std::string& source)
{
    //unsigned int id = glCreateShader(GL_VERTEX_SHADER);
    GLuint id = glCreateShader(type);

    const GLchar* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Error handling
    GLint result;
    // glGetShaderiv检查是否编译成功
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        GLint length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        //char message[length]; 会报错，因为length是变量而不是常量，不能在栈上分配此大小(堆上可以)
		GLchar* message = (GLchar*)alloca(length * sizeof(GLchar));
        // glGetShaderInfoLog获取错误消息
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

    return id;
}

static GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLuint shaderProgram = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return shaderProgram;
}

int main(void)
{
#pragma region Init

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 主版本为3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 次版本为3 即为3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 配置设为核心配置文件(core)

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

#pragma endregion

    {   // openGL自身问题：关闭窗口的时候，程序不会完全退出(check error返回一个error无限循环)。因此用作用域框起来(Cherno S13末尾)
        // 顶点位置的浮点数组(顶点不光包含位置，还有法线等信息)
        GLfloat positions[] = {
            -0.5f, -0.5f,  0.0f, // Cherno 用的是2个坐标表示一个点，改为3(与LearnOpenGL一致)
             0.5f, -0.5f,  0.0f,
             0.5f,  0.5f,  0.0f,
            -0.5f,  0.5f,  0.0f
        };

        // index buffer 索引缓冲区
        GLuint indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        // 顶点数组对象(Vertex Array Object, VAO)
        GLuint vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        // VertexBuffer对象
        VertexBuffer vb(positions, sizeof(positions));

        GLCall(glEnableVertexAttribArray(0)); // 启用顶点属性 // 设置为core后 此处会报错1282(兼容配置文件使VAO对象0为默认对象而core配置不行)，因此需要显式创建VAO(即上面的vao)
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (GLvoid*)0)); // 告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）// Cherno此处为2，改为3(与LearnOpenGL一致)

        // IndexBuffer对象
        IndexBuffer ib(indices, sizeof(indices) / sizeof(GLuint));

        // 读取shader文件
        ShaderProgramSources source = ParseShader("res/shaders/Basic.shader");
        std::cout << "VERTEX" << std::endl;
        std::cout << source.VertexSource << std::endl;
        std::cout << "FRAGMENT" << std::endl;
        std::cout << source.FragmentSource << std::endl;

        GLuint shaderProgram = CreateShader(source.VertexSource, source.FragmentSource);
        GLCall(glUseProgram(shaderProgram)); // 在glUseProgram函数调用之后，每个着色器调用和渲染调用都会使用这个程序对象

        // 获取统一变量的 location
        GLCall(GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "u_Color"));
        ASSERT(vertexColorLocation != -1); // 为 -1 意味着没有找到想要的统一变量(uniform)
        GLCall(glUniform4f(vertexColorLocation, 0.2f, 0.4f, 0.4f, 1.0f));

        GLCall(glBindVertexArray(0));
        GLCall(glUseProgram(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        //GLfloat r = 0.0f; // 红色的RGB值
        //GLfloat increment = 0.05f; // 增量

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f)); // 背景颜色

            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            GLCall(glUseProgram(shaderProgram)); // 激活着色器

            /*GLCall(glUniform4f(location, r, 0.8f, 0.8f, 1.0f));*/

            GLfloat timeValue = glfwGetTime();
            GLfloat redValue = (cos(timeValue) / 2) + 0.5;
            GLfloat greenValue = (sin(timeValue + 30) / 2) + 0.5;
            GLfloat blueValue = (sin(timeValue) / 2) + 0.5;
            glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, 1.0f);

            GLCall(glBindVertexArray(vao));

            ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // 绘制

            /*if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0)
                increment = 0.05f;

            r += increment;*/

            // 交换 前缓冲 和 后缓冲
            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));
            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }

        GLCall(glDeleteProgram(shaderProgram));
    }

    glfwTerminate();
    return 0;
}