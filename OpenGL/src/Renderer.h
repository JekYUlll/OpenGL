#ifndef RENDERER_H
#define RENDERER_H
#include "Debug.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include <imgui/imgui.h>

class Renderer
{
public:
	// ����ģʽ
	static Renderer& getInstance();

	void DrawElements(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void DrawArrays(const VertexArray& va, const Shader& shader, GLenum mode, GLint first, GLsizei count) const;

	void Clear();
	// ���ñ�����ɫ
	void SetBgColor(float r, float g, float b);
	// ���ñ�����ɫ(��͸����)
	void SetBgColor(float r, float g, float b, float a);
	// ���ñ�����ɫ(Ĭ��Ϊ��)
	void SetBgColor();
	void SetBgColor(ImVec4 clear_color);
private:
	Renderer() {}  // ˽�й��캯��
	Renderer(const Renderer&) = delete; // ��ֹ��������
	Renderer& operator=(const Renderer&) = delete; // ��ֹ��ֵ
};

#endif // !RENDERER_H


