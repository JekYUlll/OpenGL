#ifndef SHADER_H
#define SHADER_H
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <stdexcept>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "config.h"
#include "Debug.h"
#include "glm/glm.hpp"

struct ShaderProgramSources
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
private:
    unsigned int _rendererID;
    std::string _filepath;
    mutable std::unordered_map<std::string, int> _uniformLocationCache; // �洢uniform������location����ֹ����ظ���ȡ
public:
    // ��������ȡ��������ɫ��(���뵥���ļ�·��)
    Shader(const std::string& filepath);
    // ����vertex��fragment��·��
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    // ֱ�����ַ�����ʽ����shader
    Shader(const std::string& vertexShader, const std::string& fragmentShader, bool isDirect);
    ~Shader();

    void Use() const;
    void UnUse() const;

    template <typename T>
    void SetUniform(const std::string& name, const T& value) const;

    void SetUniform(const std::string& name, float v0, float v1, float v2) const;
    void SetUniform(const std::string& name, float v0, float v1, float v2, float v3) const;

    void PrintShader(const std::string& vertexShader, const std::string& fragmentShader) const; // �Լ��ӵģ���ӡshader

private:
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    int GetUniformLocation(const std::string& name) const;
    ShaderProgramSources ReadShader(const std::string& filepath); //chernoΪParseShader���˴���Ϊread�����������ذ汾
    ShaderProgramSources ReadShader(const GLchar* vertexPath, const GLchar* fragmentPath); // �ֱ������·����ȡ
    unsigned int CompileShader(unsigned int type, const std::string& source);

    void GLCheckError() const;
    std::string GetErrorString(GLenum error) const;
};

#endif // !SHADER_H
