#include "Shader.h"

Shader::Shader(const std::string& filepath)
	: _filepath(filepath), _rendererID(0)
{
    ShaderProgramSources source = ReadShader(filepath);
    this->_rendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
    ShaderProgramSources source = ReadShader(vertexPath, fragmentPath);
    this->_rendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader, bool isDirect)
    : _filepath(""), _rendererID(0)
{
    ShaderProgramSources source = { vertexShader, fragmentShader };
    this->_rendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(this->_rendererID));
}

// 读取并切分shader文件
ShaderProgramSources Shader::ReadShader(const std::string& filepath)
{
    std::ifstream stream(filepath); //
    if (!stream)
        throw std::runtime_error("Failed to open shader file: " + filepath);
    stream.exceptions(std::ifstream::badbit); // stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);会无差别在末尾抛出异常
    
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

	try {
		static unsigned int lineNum = 0; // 行数，debug用
		while (std::getline(stream, line))
		{
			lineNum++;

			if (line.find("#shader") != std::string::npos) {
				if (line.find("vertex") != std::string::npos) {
					type = ShaderType::VERTEX;
				}
				else if (line.find("fragment") != std::string::npos) {
					type = ShaderType::FRAGMENT;
				}
			}
			else {
#ifdef DEBUG_SS
                std::cout << "[line:" << lineNum << "] " << "[type:" << (int)type << "]   " << line << std::endl << std::endl; // 打印本行shader，debug用
#endif // DEBUG_SS
				ss[(int)type] << line << '\n';
			}
		}
	}
	catch (const std::ifstream::failure& e) {
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		std::cerr << "Failed to read shader file: " << filepath << std::endl;
		std::cerr << e.what() << std::endl;
		throw std::runtime_error("Shader file read error: " + filepath);
	}
    return { ss[0].str(), ss[1].str() };
}

ShaderProgramSources Shader::ReadShader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
    // 1. 从文件路径中获取顶点/片段着色器
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // 保证ifstream对象可以抛出异常：
    vShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);
    try
    {
        // 打开文件
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // 读取文件的缓冲内容到流中
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // 关闭文件
        vShaderFile.close();
        fShaderFile.close();
        // 转换流至GLchar数组
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    return { vertexCode, fragmentCode };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);

    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    // Error handling
    int result;
    // glGetShaderiv检查是否编译成功
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        //char message[length]; 会报错，因为length是变量而不是常量，不能在栈上分配此大小(堆上可以)
        char* message = (char*)alloca(length * sizeof(char));
        // glGetShaderInfoLog获取错误消息
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }
    return id;
}

void Shader::PrintShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    std::cout << "[VERTEX]" << std::endl;
    std::cout << vertexShader << std::endl;
    std::cout << "[FRAGMENT]" << std::endl;
    std::cout << fragmentShader << std::endl;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLuint shaderProgram = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(shaderProgram, vs));
    GLCall(glAttachShader(shaderProgram, fs));
    GLCall(glLinkProgram(shaderProgram));
    GLCall(glValidateProgram(shaderProgram));

    PrintShader(vertexShader, fragmentShader);

    GLCall(glDeleteShader(vs)); // 它们已经链接到我们的程序中了，已经不再需要了，因此删除
    GLCall(glDeleteShader(fs));

    return shaderProgram;
}

void Shader::Bind() const
{
    GLCall(glUseProgram(_rendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (_uniformLocationCache.find(name) != _uniformLocationCache.end())
        return _uniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(this->_rendererID, name.c_str()));

    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exit! " << std::endl; // 但即使是-1，也可以写入map中
    
    _uniformLocationCache[name] = location;
    return location;
}


