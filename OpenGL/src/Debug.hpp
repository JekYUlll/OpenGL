#pragma once
#define DEBUG_HPP

#include <iostream>
#include <GL/glew.h>

// 断言(assertion)
// __debugbreak 是 msvc 的
// from Cherno 自己用#ifdef改成了可关闭的 // Cherno 将此部分放在Renderer.h中

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
    // 调用的时候 ASSERT(GLLogCall()); 如果返回false，就会中断调试器。
}

#else
#define ASSERT(x) x;
#define GLCall(x) x;
#endif // DEBUG



