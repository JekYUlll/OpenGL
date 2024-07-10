#ifndef TEXTURE_H
#define TEXTURE_H

#include "Debug.h"
#include "Renderer.h"

class Texture
{
private:
	unsigned int _rendererID;
	std::string _filePath; // 纹理的路径
	unsigned char* _localBuffer; // 纹理的本地存储
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

