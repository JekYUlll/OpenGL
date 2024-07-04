#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// ����(assertion)
// __debugbreak �� msvc ��
#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

// ��Ļ�� ��
const GLuint WIDTH = 640, HEIGHT = 480;

// from LearnOpenGL �̳��к�������key_callback
static void KeyCallback_exit(GLFWwindow * window, int key, int scancode, int action, int mode)
{
    // ���û�����ESC��,��������window���ڵ�WindowShouldClose����Ϊtrue
    // �ر�Ӧ�ó���
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
    // ���õ�ʱ�� ASSERT(GLLogCall()); �������false���ͻ��жϵ�������
}

struct ShaderProgramSources
{
    std::string VertexSource;
    std::string FragmentSource;
};

// ��ȡ���з�shader�ļ�
static ShaderProgramSources ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);
	// ���ڶ�ȡ��shader����
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
    // glGetShaderiv����Ƿ����ɹ�
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        GLint length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        //char message[length]; �ᱨ����Ϊlength�Ǳ��������ǳ�����������ջ�Ϸ���˴�С(���Ͽ���)
		GLchar* message = (GLchar*)alloca(length * sizeof(GLchar));
        // glGetShaderInfoLog��ȡ������Ϣ
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

    // from LearnOpenGL ͨ��GLFWע�����ǵĺ��������ʵĻص�
    glfwSetKeyCallback(window, KeyCallback_exit);

    glfwSwapInterval(1); // ���ô�ֱͬ����֡���޶���60

    // GLEW
    if (glewInit() != GLEW_OK)
        std::cout << "GLEW init Error!" << std::endl;
    
    //  ��ӡ�汾��
    std::cout << "GLEW version: " << glGetString(GL_VERSION) << std::endl;
#pragma endregion

    // ����λ�õĸ�������(���㲻�����λ�ã����з��ߵ���Ϣ)
    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f,
    };

    // index buffer ����������
    GLuint indices[] = {
        0, 1, 2, 
        2, 3, 0
    };

    // ���㻺����
    GLuint buffer;
    // ʹ��glGenBuffers������һ������ID����һ�� ���㻺�����(Vertex Buffer Objects, VBO)
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    // ��֮ǰ����Ķ������ݸ��Ƶ�������ڴ���
    // ���ĸ�����ָ��������ϣ���Կ���ι�����������ݡ�����������ʽ��
        // GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
        // GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
        // GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0)); // ���ö�������
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (GLvoid*)0)); // ����OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ�

    // index buffer object
    GLuint ibo;
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    //glBindBuffer(GL_ARRAY_BUFFER, 0);

#pragma region ���ַ�����ʽ��дshader
    // �ַ����ķ�ʽ����shader
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

    // ��ȡshader�ļ�
    ShaderProgramSources source = ParseShader("res/shaders/Basic.shader");
    std::cout << "VERTEX" << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "FRAGMENT" << std::endl;
    std::cout << source.FragmentSource << std::endl;
  
    GLuint shaderProgram = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shaderProgram)); // ��glUseProgram��������֮��ÿ����ɫ�����ú���Ⱦ���ö���ʹ������������

    // ��ȡͳһ������ location
    GLCall(int location = glGetUniformLocation(shaderProgram, "u_Color"));
    ASSERT(location != -1); // location Ϊ -1 ��ζ��û���ҵ���Ҫ��ͳһ����(uniform)
    GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));

    float r = 0.0f; // ��ɫ��RGBֵ
    float increment = 0.05f; // ����

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT); 

#pragma region ��ͳOpenGL��һ��������
        /*glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();*/
#pragma endregion

#pragma region ����������
        //glDrawArrays(GL_TRIANGLES, 0, 6);
#pragma endregion

        // ʵʱ�ı���ɫ
        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // ����

        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0)
            increment = 0.05f;

        r += increment;

        // ���� ǰ���� �� �󻺳�
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