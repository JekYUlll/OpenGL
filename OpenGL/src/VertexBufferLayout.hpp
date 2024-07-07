#pragma once
#define VERTEXBUFFERLAYOUT_HPP
#include <vector>
#include <GL/glew.h>
#include "Debug.hpp"

struct VertexBufferElement
{
	unsigned int type; // �������ݵ���������
	unsigned int count; // ÿ�����������ݵ�����
	unsigned char normalized; // �Ƿ���Ҫ��׼��

	// ��ȡ������ռ�ֽ���
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

// ���嶥�㻺�����Ĳ��֣������������Ե����͡������Ͳ�������Ϣ��
class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> _elements;
	unsigned int _stride; // ����

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
		this->_stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT); // ֮ǰ���˳�count�����»��Ƴ����⣬���˰���
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
		this->_elements.push_back({ GL_UNSIGNED_BYTE, count, static_cast<unsigned char>(GL_TRUE) }); // 	C2397	�ӡ�int��ת������unsigned int����Ҫ����ת��	����˶�GL_TRUE��������ת��
		this->_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement>& GetElements() const { return this->_elements; } // Cherno�˴�δ�������ö��ǿ���
	inline unsigned int GetStride() const { return this->_stride; }
};
