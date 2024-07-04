#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// 断言(assertion)
// __debugbreak 是 msvc 的
#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

// 屏幕宽 高
const GLuint WIDTH = 640, HEIGHT = 480;

// from LearnOpenGL 教程中函数名是key_callback
static void KeyCallback_exit(GLFWwindow * window, int key, int scancode, int action, int mode)
{
    // 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
    // 关闭应用程序
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file  << " : " << line << std::endl;
        return false;
    }
    return true;
    // 调用的时候 ASSERT(GLLogCall()); 如果返回false，就会中断调试器。
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
			ss[(int)type] << line << '\n';
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
#pragma region 0
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Cherno OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // from LearnOpenGL 通过GLFW注册我们的函数至合适的回调
    glfwSetKeyCallback(window, KeyCallback_exit);

    glfwSwapInterval(1); // 设置垂直同步，帧数限定在60

    // GLEW
    if (glewInit() != GLEW_OK)
        std::cout << "GLEW init Error!" << std::endl;
    
    //  打印版本号
    std::cout << "GLEW version: " << glGetString(GL_VERSION) << std::endl;
#pragma endregion

    // 顶点位置的浮点数组(顶点不光包含位置，还有法线等信息)
    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f,
    };

    // index buffer 索引缓冲区
    GLuint indices[] = {
        0, 1, 2, 
        2, 3, 0
    };

    // 顶点缓冲区
    GLuint buffer;
    // 使用glGenBuffers函数和一个缓冲ID生成一个 顶点缓冲对象(Vertex Buffer Objects, VBO)
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    // 把之前定义的顶点数据复制到缓冲的内存中
    // 第四个参数指定了我们希望显卡如何管理给定的数据。它有三种形式：
        // GL_STATIC_DRAW ：数据不会或几乎不会改变。
        // GL_DYNAMIC_DRAW：数据会被改变很多。
        // GL_STREAM_DRAW ：数据每次绘制时都会改变。
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0)); // 启用顶点属性
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (GLvoid*)0)); // 告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）

    // index buffer object
    GLuint ibo;
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    //glBindBuffer(GL_ARRAY_BUFFER, 0);

#pragma region 以字符串方式编写shader
    // 字符串的方式传入shader
    //std::string vertexShader =
    //    "#version 330 core\n"
    //    "\n"
    //    "layout(location = 0) in vec4 position;\n"
    //    "\n"
    //    "void main()\n"
    //    "{\n"
    //    "   gl_Position = position;\n"
    //    "}\n";

    //std::string fragmentShader =
    //    "#version 330 core\n"
    //    "\n"
    //    "layout(location = 0) out vec4 color;\n"
    //    "\n"
    //    "void main()\n"
    //    "{\n"
    //    "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
    //    "}\n";

    
#pragma endregion

    // 读取shader文件
    ShaderProgramSources source = ParseShader("res/shaders/Basic.shader");
    std::cout << "VERTEX" << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "FRAGMENT" << std::endl;
    std::cout << source.FragmentSource << std::endl;
  
    GLuint shaderProgram = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shaderProgram)); // 在glUseProgram函数调用之后，每个着色器调用和渲染调用都会使用这个程序对象

    // 获取统一变量的 location
    GLCall(int location = glGetUniformLocation(shaderProgram, "u_Color"));
    ASSERT(location != -1); // location 为 -1 意味着没有找到想要的统一变量(uniform)
    GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));

    float r = 0.0f; // 红色的RGB值
    float increment = 0.05f; // 增量

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT); 

#pragma region 传统OpenGL画一个三角形
        /*glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();*/
#pragma endregion

#pragma region 绘制三角形
        //glDrawArrays(GL_TRIANGLES, 0, 6);
#pragma endregion

        // 实时改变颜色
        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // 绘制

        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0)
            increment = 0.05f;

        r += increment;

        // 交换 前缓冲 和 后缓冲
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    //
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}