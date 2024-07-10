#ifndef TEXTURE_H
#define TEXTURE_H

#include "Debug.h"
#include "Renderer.h"

class Texture
{
private:
	unsigned int _rendererID;
	std::string _filePath; // �����·��
	unsigned char* _localBuffer; // ����ı��ش洢
	int _width, _height;
	int _BPP; // bits per pixel

public:
	Texture(const std::string& filePath);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void UnBind();

	inline int GetWidth() const { return this->_width; }
	inline int GetHeight() const { return this->_height; }
};


















#endif // !TEXTURE_H

