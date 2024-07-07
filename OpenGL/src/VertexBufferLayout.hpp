#pragma once
#define VERTEXBUFFERLAYOUT_HPP
#include <vector>
#include <GL/glew.h>
#include "Debug.hpp"

struct VertexBufferElement
{
	unsigned int type; // 顶点数据的数据类型
	unsigned int count; // 每个顶点中数据的数量
	unsigned char normalized; // 是否需要标准化

	// 获取类型所占字节数
	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:         return 4;
		case GL_UNSIGNED_INT:  return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
		return 0;
	}
};

typedef std::vector<VertexBufferElement> Buffers;

// 定义顶点缓冲区的布局，包括顶点属性的类型、数量和步幅等信息。
class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> _elements;
	unsigned int _stride; // 步幅

public:
	VertexBufferLayout()
		: _stride(0) {}
	
	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		_elements.push_back({ GL_FLOAT, count, static_cast<unsigned char>(GL_FALSE) });
		this->_stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT); // 之前忘了乘count，导致绘制出问题，找了半天
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		this->_elements.push_back({ GL_UNSIGNED_INT, count, static_cast<unsigned char>(GL_FALSE) });
		this->_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		this->_elements.push_back({ GL_UNSIGNED_BYTE, count, static_cast<unsigned char>(GL_TRUE) }); // 	C2397	从“int”转换到“unsigned int”需要收缩转换	，因此对GL_TRUE进行类型转换
		this->_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement>& GetElements() const { return this->_elements; } // Cherno此处未返回引用而是拷贝
	inline unsigned int GetStride() const { return this->_stride; }
};
