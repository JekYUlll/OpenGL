#include "Debug.h"

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const GLchar* function, const GLchar* file, GLint line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << " : " << line << std::endl;
        return false;
    }
    return true;
    // ���õ�ʱ�� ASSERT(GLLogCall()); �������false���ͻ��жϵ�������
}