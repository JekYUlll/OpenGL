#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    // ʹ��glGenBuffers������һ������ID����һ�� ���㻺�����(Vertex Buffer Objects, VBO)
    GLCall(glGenBuffers(1, &_rendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, _rendererID));
    // ��֮ǰ����Ķ������ݸ��Ƶ�������ڴ���
    // ���ĸ�����ָ��������ϣ���Կ���ι�����������ݡ�����������ʽ��
        // GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
        // GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
        // GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &_rendererID));
}

// �� vertex buffer
void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, _rendererID));
}

// ��� vertex buffer
void VertexBuffer::UnBind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
