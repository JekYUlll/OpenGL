#pragma once
#define DEBUG_HPP

#include <iostream>
#include <GL/glew.h>

// ����(assertion)
// __debugbreak �� msvc ��
// from Cherno �Լ���#ifdef�ĳ��˿ɹرյ� // Cherno ���˲��ַ���Renderer.h��

#ifdef DEBUG
#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const GLchar * function, const GLchar * file, GLint line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << " : " << line << std::endl;
        return false;
    }
    return true;
    // ���õ�ʱ�� ASSERT(GLLogCall()); �������false���ͻ��жϵ�������
}

#else
#define ASSERT(x) x;
#define GLCall(x) x;
#endif // DEBUG



