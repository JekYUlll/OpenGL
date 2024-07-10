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
#include "Debug.h"

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
    std::unordered_map<std::string, int> _uniformLocationCache; // �洢uniform������location����ֹ����ظ���ȡ
public:
    // ��������ȡ��������ɫ��(���뵥���ļ�·��)
    Shader(const std::string& filepath);
    // ����vertex��fragment��·��
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    // ֱ�����ַ�����ʽ����shader
    Shader(const std::string& vertexShader, const std::string& fragmentShader, bool isDirect);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // todo: 1.�˴�����template������������͡� 2.���Խ�����ѧ�⣬���㴫������ 3.д��������ú���
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform1i(const std::string& name, int value);

private:
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    int GetUniformLocation(const std::string& name);
    ShaderProgramSources ReadShader(const std::string& filepath); //chernoΪParseShader���˴���Ϊread�����������ذ汾
    ShaderProgramSources ReadShader(const GLchar* vertexPath, const GLchar* fragmentPath); // �ֱ������·����ȡ
    unsigned int CompileShader(unsigned int type, const std::string& source);
    void PrintShader(const std::string& vertexShader, const std::string& fragmentShader); // �Լ��ӵģ���ӡshader
};

#endif // !SHADER_H



