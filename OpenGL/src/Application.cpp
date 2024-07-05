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

// ��Ļ�� ��
const GLuint WIDTH = 640, HEIGHT = 480;

// ��ӡ��ʼ��Ϣ (�Լ���д)
static void PrintInitInfo() {
    // ��ӡ�汾��
    std::cout << "GLEW version: " << glGetString(GL_VERSION) << std::endl;
    // ��ӡ�ֱ���
    std::cout << "resolution: " << WIDTH << " * " << HEIGHT << std::endl;
    // ��ӡ������������
    GLint nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "maximum nr of vertex attributes supported: " << nrAttributes << std::endl << std::endl;
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
#pragma region Init

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // ���汾Ϊ3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // �ΰ汾Ϊ3 ��Ϊ3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // ������Ϊ���������ļ�(core)

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Cherno OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // from LearnOpenGL ͨ��GLFWע�����ǵĺ��������ʵĻص� (����Ϊ��������ӣ���������ʵ����ֻ�ܰ�һ�������ֻ��Ҫһ��CallBack����)
    GLCall(glfwSetKeyCallback(window, kcb::KeyCallBack));

    glfwSwapInterval(1); // ���ô�ֱͬ����֡���޶���60

    // GLEW
    if (glewInit() != GLEW_OK)
        std::cout << "GLEW init Error!" << std::endl;
    
    PrintInitInfo();

#pragma endregion

    {   // openGL�������⣺�رմ��ڵ�ʱ�򣬳��򲻻���ȫ�˳�(check error����һ��error����ѭ��)������������������(Cherno S13ĩβ)
        // ����λ�õĸ�������(���㲻�����λ�ã����з��ߵ���Ϣ)
        GLfloat positions[] = {
            -0.5f, -0.5f,  0.0f, // Cherno �õ���2�������ʾһ���㣬��Ϊ3(��LearnOpenGLһ��)
             0.5f, -0.5f,  0.0f,
             0.5f,  0.5f,  0.0f,
            -0.5f,  0.5f,  0.0f
        };

        // index buffer ����������
        GLuint indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        // �����������(Vertex Array Object, VAO)
        GLuint vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        // VertexBuffer����
        VertexBuffer vb(positions, sizeof(positions));

        GLCall(glEnableVertexAttribArray(0)); // ���ö������� // ����Ϊcore�� �˴��ᱨ��1282(���������ļ�ʹVAO����0ΪĬ�϶����core���ò���)�������Ҫ��ʽ����VAO(�������vao)
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (GLvoid*)0)); // ����OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ�// Cherno�˴�Ϊ2����Ϊ3(��LearnOpenGLһ��)

        // IndexBuffer����
        IndexBuffer ib(indices, sizeof(indices) / sizeof(GLuint));

        // ��ȡshader�ļ�
        ShaderProgramSources source = ParseShader("res/shaders/Basic.shader");
        std::cout << "VERTEX" << std::endl;
        std::cout << source.VertexSource << std::endl;
        std::cout << "FRAGMENT" << std::endl;
        std::cout << source.FragmentSource << std::endl;

        GLuint shaderProgram = CreateShader(source.VertexSource, source.FragmentSource);
        GLCall(glUseProgram(shaderProgram)); // ��glUseProgram��������֮��ÿ����ɫ�����ú���Ⱦ���ö���ʹ������������

        // ��ȡͳһ������ location
        GLCall(GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "u_Color"));
        ASSERT(vertexColorLocation != -1); // Ϊ -1 ��ζ��û���ҵ���Ҫ��ͳһ����(uniform)
        GLCall(glUniform4f(vertexColorLocation, 0.2f, 0.4f, 0.4f, 1.0f));

        GLCall(glBindVertexArray(0));
        GLCall(glUseProgram(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        //GLfloat r = 0.0f; // ��ɫ��RGBֵ
        //GLfloat increment = 0.05f; // ����

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f)); // ������ɫ

            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            GLCall(glUseProgram(shaderProgram)); // ������ɫ��

            /*GLCall(glUniform4f(location, r, 0.8f, 0.8f, 1.0f));*/

            GLfloat timeValue = glfwGetTime();
            GLfloat redValue = (cos(timeValue) / 2) + 0.5;
            GLfloat greenValue = (sin(timeValue + 30) / 2) + 0.5;
            GLfloat blueValue = (sin(timeValue) / 2) + 0.5;
            glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, 1.0f);

            GLCall(glBindVertexArray(vao));

            ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // ����

            /*if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0)
                increment = 0.05f;

            r += increment;*/

            // ���� ǰ���� �� �󻺳�
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