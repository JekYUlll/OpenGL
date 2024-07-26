#shader vertex --------------------------------
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aNormal;
layout(location = 4) in float aTexIndex;

out vec3 objectColor;
out vec2 TexCoord;
out float TexIndex;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    objectColor = aColor;
    TexCoord = aTexCoord;
    TexIndex = aTexIndex;
    FragPos = vec3(model * vec4(aPos, 1.0f));
    Normal = mat3(transpose(inverse(model))) * aNormal; // ���·���(ת��������ռ�)
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}

#shader fragment --------------------------------
#version 330 core

out vec4 color;

in vec3 objectColor;
in vec2 TexCoord;
in float TexIndex;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 pureObjectColor;
uniform int debugMode; // 0: normal, 1: visualize normal, 2: visualize frag position

void main()
{
    if(debugMode == 0) 
    {
        vec4 texColor1 = texture(ourTexture1, TexCoord);
        vec4 texColor2 = texture(ourTexture2, TexCoord);
  
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
  
        float ambientStrength = 0.1f;
        vec3 ambient = ambientStrength * lightColor;
  
        float specularStrength = 0.5f;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // 32�Ǹ߹�ķ���ֵ(Shininess)
        vec3 specular = specularStrength * spec * lightColor;

        vec3 result = (ambient + diffuse + specular) * pureObjectColor;
        color = vec4(result, 1.0f);
    } 
    else if (debugMode == 1) 
    {
        // ���ӻ�����
        vec3 norm = normalize(Normal);
        vec3 debugColor = (norm * 0.5) + 0.5;
        color = vec4(debugColor, 1.0f);
    } 
    else if (debugMode == 2) 
    {
        // ���ӻ�Ƭ��λ��
        vec3 debugColor = min(FragPos, 1.0);
        color = vec4(debugColor, 1.0f);
    }
}