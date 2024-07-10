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
    std::unordered_map<std::string, int> _uniformLocationCache; // 存储uniform变量的location，防止多次重复获取
public:
    // 构造器读取并构建着色器(传入单个文件路径)
    Shader(const std::string& filepath);
    // 传入vertex和fragment的路径
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    // 直接用字符串形式传入shader
    Shader(const std::string& vertexShader, const std::string& fragmentShader, bool isDirect);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // todo: 1.此处可用template，传入更多类型。 2.可以接入数学库，方便传入向量 3.写更多的设置函数
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform1i(const std::string& name, int value);

private:
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    int GetUniformLocation(const std::string& name);
    ShaderProgramSources ReadShader(const std::string& filepath); //cherno为ParseShader，此处改为read，并加入重载版本
    ShaderProgramSources ReadShader(const GLchar* vertexPath, const GLchar* fragmentPath); // 分别从两个路径读取
    unsigned int CompileShader(unsigned int type, const std::string& source);
    void PrintShader(const std::string& vertexShader, const std::string& fragmentShader); // 自己加的，打印shader
};

#endif // !SHADER_H



