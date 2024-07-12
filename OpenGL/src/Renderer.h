#ifndef RENDERER_H
#define RENDERER_H
#include "Debug.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
public:
	// 单例模式
	static Renderer& getInstance();

	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

	void Clear();
	// 设置背景颜色
	void SetBgColor(float r, float g, float b);
	// 设置背景颜色(带透明度)
	void SetBgColor(float r, float g, float b, float a);
	// 设置背景颜色(默认为黑)
	void SetBgColor();
private:
	Renderer() {}  // 私有构造函数
	Renderer(const Renderer&) = delete; // 禁止拷贝构造
	Renderer& operator=(const Renderer&) = delete; // 禁止赋值
};

#endif // !RENDERER_H


