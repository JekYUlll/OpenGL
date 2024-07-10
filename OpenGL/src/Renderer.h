#ifndef RENDERER_H
#define RENDERER_H
#include "Debug.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

// todo: �ĳɵ���ģʽ
class Renderer
{
public:
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

	void Clear();
	// ���ñ�����ɫ
	void SetBgColor(float r, float g, float b);
	// ���ñ�����ɫ(��͸����)
	void SetBgColor(float r, float g, float b, float a);
	// ���ñ�����ɫ(Ĭ��Ϊ��)
	void SetBgColor();
};

#endif // !RENDERER_H


