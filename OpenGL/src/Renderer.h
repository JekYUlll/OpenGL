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
	// ���ñ�����ɫ
	void SetBgColor(float r, float g, float b);
	// ���ñ�����ɫ(Ĭ��Ϊ��)
	void SetBgColor();
};


