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

// ��ȡ���з�shader�ļ�
ShaderProgramSources Shader::ReadShader(const std::string& filepath)
{
    std::ifstream stream(filepath); //
    if (!stream)
        throw std::runtime_error("Failed to open shader file: " + filepath);
    stream.exceptions(std::ifstream::badbit); // stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);���޲����ĩβ�׳��쳣
    
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

	try {
		static unsigned int lineNum = 0; // ������debug��
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
                std::cout << "[line:" << lineNum << "] " << "[type:" << (int)type << "]   " << line << std::endl << std::endl; // ��ӡ����shader��debug��
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
    // 1. ���ļ�·���л�ȡ����/Ƭ����ɫ��
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ��֤ifstream��������׳��쳣��
    vShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);
    try
    {
        // ���ļ�
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // ��ȡ�ļ��Ļ������ݵ�����
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // �ر��ļ�
        vShaderFile.close();
        fShaderFile.close();
        // ת������GLchar����
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
    // glGetShaderiv����Ƿ����ɹ�
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        //char message[length]; �ᱨ����Ϊlength�Ǳ��������ǳ�����������ջ�Ϸ���˴�С(���Ͽ���)
        char* message = (char*)alloca(length * sizeof(char));
        // glGetShaderInfoLog��ȡ������Ϣ
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

    GLCall(glDeleteShader(vs)); // �����Ѿ����ӵ����ǵĳ������ˣ��Ѿ�������Ҫ�ˣ����ɾ��
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
        std::cout << "Warning: uniform '" << name << "' doesn't exit! " << std::endl; // ����ʹ��-1��Ҳ����д��map��
    
    _uniformLocationCache[name] = location;
    return location;
}


