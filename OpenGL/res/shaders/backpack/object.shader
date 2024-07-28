#shader vertex --------------------------------
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    Normal = mat3(transpose(inverse(model))) * normal; 
    TexCoords = texCoords;
}

#shader fragment --------------------------------
#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform sampler2D test_texture;

void main()
{   
    vec4 diffuseColor = texture(texture_diffuse1, TexCoords);
    vec4 specularColor = texture(texture_specular1, TexCoords);

    vec4 testColor = texture(test_texture, TexCoords);

    color = vec4(TexCoords, 0.0, 1.0); // 将纹理坐标作为颜色输出
    color = diffuseColor;

    if (TexCoords.x < 100.0) {
        color = diffuseColor;
    } else if (TexCoords.x < 200.0) {
        color = specularColor;
    } else {
        color = testColor;
    }
}