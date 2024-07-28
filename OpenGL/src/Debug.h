#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>
#include <GL/glew.h>
// ����(assertion) __debugbreak �� msvc �� Cherno ���˲��ַ���Renderer.h��
#ifdef _DEBUG

#ifdef _MSC_VER
#define ASSERT(x) if(!(x)) __debugbreak();
#else
#define ASSERT(x) x;
#endif // _MSC_VER

#define FUNC_CALL std::cout << __func__ << "Called" << std::endl

#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const GLchar * function, const GLchar * file, GLint line);

#else
#define ASSERT(x) x;
#define GLCall(x) x;
#define FUNC_CALL

#endif // _DEBUG



#endif // !DEBUG_HPP



