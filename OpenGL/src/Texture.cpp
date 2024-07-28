#include "Texture.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

void Texture::DefineTextureType()
{
	this->_type = (typeName == "texture_diffuse") ? TextureType::DIFFUSE : (typeName == "texture_specular") ? TextureType::SPECULAR : TextureType::NONE;
}

Texture::Texture(const std::string& filePath)
	: _rendererID(0), _filePath(filePath), _localBuffer(nullptr), _width(0), _height(0), _BPP(0), _type(TextureType::NONE)
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
#ifdef _DEBUG
		std::cout << "Succeed to load texture: \"" << _filePath << "\". " << "" << _width << " * " << _height << ", BPP: " << _BPP << std::endl;
#endif // _DEBUG
		GLCall(glGenTextures(1, &_rendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, _rendererID));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _localBuffer)); // Cherno�̳�������ڱȽϺ����λ��  GL_RGBA8 ��ʾ�ڲ���ʽΪ 8 λ�� RGBA���졢�̡�����͸��ͨ������ʽ������ζ��ÿ����ɫͨ������ 8 λ��1 ���ֽڣ��Ĵ洢�ռ䣬���� 32 λ��4 ���ֽڣ��洢һ�����ء�ʹ�����ָ�ʽʱ��ÿ�����ض���һ����ɫ��һ����ɫ��һ����ɫ��һ��͸��ͨ������������Ҫ͸������Ϣ�������������͸��������ͼ��
		// xxx �ж���Ⱦ����
		if (Renderer::getInstance().GetRenderMode() == RenderMode::MODEL) {
			GLCall(glGenerateMipmap(GL_TEXTURE_2D));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		}
		else if (Renderer::getInstance().GetRenderMode() == RenderMode::CUBE) {
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		}
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		DefineTextureType(); // xx������������ (�˴����ú���ûʲô���ã�����NONE)
	}
	else {
		std::cerr << "Failed to load texture: " << _filePath << std::endl;
	}
	if (_localBuffer) {
		stbi_image_free(_localBuffer);
	}
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &_rendererID)); // ????
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot)); // OpenGL���ٱ�֤��16������Ԫ��ʹ�ã���GL_TEXTURE0��GL_TEXTRUE15������ͨ��GL_TEXTURE0 + 8�ķ�ʽ���GL_TEXTURE8
	GLCall(glBindTexture(GL_TEXTURE_2D, _rendererID)); // �����������ǰ���������Ԫ������ԪGL_TEXTURE0Ĭ�����Ǳ����
	// xxx ��֪���費��Ҫ�ع���LearnOpenGL�� Mesh::Draw() �У���  glActiveTexture ��  glBindTexture �м���õ� glUniform1i�����ҵ�ʵ����������������Դ��������ָ�룬���м����SetUniform����SetUniform�Ƿ��ͣ�д����ָ��Ӧ�ú���(����֪���Ƿ����)
}

void Texture::UnBind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
