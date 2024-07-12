#define DEBUG
#include "Texture.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& filePath)
	: _rendererID(0), _filePath(filePath), _localBuffer(nullptr), _width(0), _height(0), _BPP(0)
{
	// 检查文件是否存在
	std::ifstream file(filePath);
	if (!file.good()) {
		std::cerr << "Texture file does not exist: " << filePath << std::endl;
		return;
	}
	file.close();

	stbi_set_flip_vertically_on_load(1); // OpenGL (0,0) 在左下角，所以需要翻转
	_localBuffer = stbi_load(_filePath.c_str(), &_width, &_height, &_BPP, 4); // 使用RGBA，所以传入4
	
	if (_localBuffer) {
		std::cout << "Succeed to load texture: " << _filePath << std::endl;
		GLCall(glGenTextures(1, &_rendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, _rendererID));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _localBuffer));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
	else {
		std::cerr << "Failed to load texture: " << _filePath << std::endl;
	}
	if (_localBuffer)
		stbi_image_free(_localBuffer);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &_rendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot)); // OpenGL至少保证有16个纹理单元供使用，从GL_TEXTURE0到GL_TEXTRUE15。可以通过GL_TEXTURE0 + 8的方式获得GL_TEXTURE8
	GLCall(glBindTexture(GL_TEXTURE_2D, _rendererID)); // 绑定这个纹理到当前激活的纹理单元，纹理单元GL_TEXTURE0默认总是被激活。
}

void Texture::UnBind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
