#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Debug.h"
#include "Renderer.h"

enum class TextureType
{
	NONE = -1,
	DIFFUSE,
	SPECULAR
};

// �����ع�һ�£����Texture��һ����Ա��TextureData������̳��е�Texture�ṹ��
struct TextureData
{
	unsigned int id; // �� _rendererID
	std::string type; // �� typeName
	aiString path; 
};

class Texture
{
private:
	unsigned int _rendererID;
	std::string _filePath; // �����·��
	unsigned char* _localBuffer; // ����ı��ش洢
	int _width, _height;
	int _BPP; // bits per pixel

public:
	std::string typeName;
	aiString path;

private:
	TextureType _type;
	TextureData _textureData;

public:
	Texture(const std::string& filePath);
	~Texture();

	void DefineTextureType(); // ��������string typeName���ı������ö��TextureType _type

	void Bind(unsigned int slot = 0) const;
	void UnBind() const;

	inline int GetWidth() const { return this->_width; }
	inline int GetHeight() const { return this->_height; }
	inline TextureType GetType() const { return this->_type; }
	inline void SetType(const TextureType& type) { this->_type = type; }
	inline TextureData GetTextureData() const { return this->_textureData; }
	inline void SetTextureData(const TextureData& textureData) { this->_textureData = textureData; }
	inline unsigned int GetID() const { return this->_rendererID; }

};




















#endif // !TEXTURE_H

