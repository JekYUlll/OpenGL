#ifndef RENDERER_H
#define RENDERER_H
#include "Debug.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

// todo: 改成单例模式
class Renderer
{
public:
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

	void Clear();
	// 设置背景颜色
	void SetBgColor(float r, float g, float b);
	// 设置背景颜色(带透明度)
	void SetBgColor(float r, float g, float b, float a);
	// 设置背景颜色(默认为黑)
	void SetBgColor();
};

#endif // !RENDERER_H


