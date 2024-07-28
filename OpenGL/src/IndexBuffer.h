#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H
#include "Debug.h"

class IndexBuffer
{
private:
	unsigned int _rendererID;
	unsigned int _count; // �ж�������
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;

	// ��ȡ��������
	inline unsigned int GetCount() const { return _count; }
};

#endif // !INDEXBUFFER_H