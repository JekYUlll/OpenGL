#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H
#include "VertexData.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
	unsigned int _rendererID;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const;

	void Bind() const;
	void Unbind() const;
};

#endif // !VERTEXARRAY_H