//��Ҫ����ո� :(
#type vertex
#version 430 core
layout (location = 0) in vec3 i_Pos;
layout (location = 1) in vec3 i_Normal;
layout (location = 2) in vec2 i_TexCoords;

// declare an interface block; see 'Advanced GLSL' for what these are.
out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    vs_out.FragPos = i_Pos;
    vs_out.Normal = i_Normal;
    vs_out.TexCoords = i_TexCoords;
    gl_Position = projection * view * vec4(i_Pos, 1.0);
}


#type fragment
#version 430 core
layout(location = 0) out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D texture1;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool blinn;

void main()
{           
    vec3 color = texture(texture1, fs_in.TexCoords).rgb;
    // ����
    vec3 ambient = 0.15 * color;
    // ������
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    vec3 normal = normalize(fs_in.Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // ����
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;

    if(blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }
    vec3 specular = vec3(0.5) * spec; // assuming bright white light color
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}