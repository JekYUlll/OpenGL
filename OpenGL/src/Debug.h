#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>
#include <GL/glew.h>
// 断言(assertion) __debugbreak 是 msvc 的 Cherno 将此部分放在Renderer.h中
#ifdef _DEBUG
#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const GLchar * function, const GLchar * file, GLint line);

#else
#define ASSERT(x) x;
#define GLCall(x) x;

#endif // _DEBUG

#endif // !DEBUG_HPP



