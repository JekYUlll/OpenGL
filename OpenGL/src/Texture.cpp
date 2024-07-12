#define DEBUG
#include "Texture.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& filePath)
	: _rendererID(0), _filePath(filePath), _localBuffer(nullptr), _width(0), _height(0), _BPP(0)
{
	// ����ļ��Ƿ����
	std::ifstream file(filePath);
	if (!file.good()) {
		std::cerr << "Texture file does not exist: " << filePath << std::endl;
		return;
	}
	file.close();

	stbi_set_flip_vertically_on_load(1); // OpenGL (0,0) �����½ǣ�������Ҫ��ת
	_localBuffer = stbi_load(_filePath.c_str(), &_width, &_height, &_BPP, 4); // ʹ��RGBA�����Դ���4
	
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
	GLCall(glActiveTexture(GL_TEXTURE0 + slot)); // OpenGL���ٱ�֤��16������Ԫ��ʹ�ã���GL_TEXTURE0��GL_TEXTRUE15������ͨ��GL_TEXTURE0 + 8�ķ�ʽ���GL_TEXTURE8
	GLCall(glBindTexture(GL_TEXTURE_2D, _rendererID)); // �����������ǰ���������Ԫ������ԪGL_TEXTURE0Ĭ�����Ǳ����
}

void Texture::UnBind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
