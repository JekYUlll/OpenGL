#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
    : vertices(vertices), indices(indices), textures(textures)
{
    _VAO = std::make_unique<VertexArray>();
    _VBO = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(Vertex));
    _IBO = std::make_unique<IndexBuffer>(indices.data(), indices.size());
    _VAO->Bind();
    _VBO->Bind();
    _IBO->Bind();
    // ÈßÓà£¬²âÊÔ
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);
    VertexBufferLayout layout = VertexLayoutBuilder<Vertex>::Build();
    _VAO->AddBuffer(*_VBO, layout);
    _VAO->Unbind();
    // ???
    _VBO->UnBind();
    _IBO->UnBind();
}

void Mesh::Draw(const Shader& shader) const
{
    unsigned int difNum = 0;
    unsigned int speNum = 0;
    shader.Use();

    for (int i = 0; i < textures.size(); i++)
    {
        textures[i].Bind(i);

        if (glIsTexture(textures[i].GetID()) == GL_FALSE) {
            std::cerr << "Texture ID is not valid for texture " << i << std::endl;
        }

        switch (textures[i].GetType())
        {
        case TextureType::NONE:
            std::cerr << "Texture " << i << " has no type!" << std::endl;
            break;
        case TextureType::DIFFUSE:
            difNum++;
            shader.SetUniform("texture_diffuse" + std::to_string(difNum), i);
            break;
        case TextureType::SPECULAR:
            speNum++;
            shader.SetUniform("texture_specular" + std::to_string(speNum), i);
            break;
        default:
            break;
        }
    }
#ifdef _DEBUG
    static bool firstCall = true;
    if (firstCall) {
        std::cout << difNum << " Diffuse Texture, " << speNum << " Specular Texture Bind" << std::endl;
        firstCall = false;
    }
#endif // !_DEBUG

    // Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
    shader.SetUniform<float>("material.shininess", 16.0f);

    _VAO->Bind();
    Renderer::getInstance().DrawElements(*_VAO, *_IBO, shader);
    _VAO->Unbind();

    for (const Texture& texture : textures) {
        texture.UnBind();
    }
}




