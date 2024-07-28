#ifndef RENDERER_H
#define RENDERER_H

#include "Debug.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include <imgui/imgui.h>

enum class RenderMode
{
	MODEL,
	CUBE
};

class Renderer
{
private:
	RenderMode _renderMode;
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

	inline RenderMode GetRenderMode() const { return this->_renderMode; }
	// �л���������ģʽ����ı� Texture ���һЩʵ��
	inline void SwitchToCube() { this->_renderMode = RenderMode::CUBE; }
private:
	Renderer() : _renderMode(RenderMode::MODEL) {}  // ˽�й��캯��
	Renderer(const Renderer&) = delete; // ��ֹ��������
	Renderer& operator=(const Renderer&) = delete; // ��ֹ��ֵ
};

#endif // !RENDERER_H


