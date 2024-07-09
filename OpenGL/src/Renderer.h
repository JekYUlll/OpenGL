#pragma once

#include "Debug.hpp"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
public:
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

	void Clear();
	// 设置背景颜色
	void SetBgColor(float r, float g, float b);
	// 设置背景颜色(默认为黑)
	void SetBgColor();
};


