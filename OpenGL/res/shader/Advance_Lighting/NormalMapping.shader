#type vertex
#version 330 core
layout (location = 0) in vec3 i_Pos;
layout (location = 1) in vec3 i_Normal;
layout (location = 2) in vec2 i_Texcoords;
layout (location = 3) in vec3 i_Tangent;
layout (location = 4) in vec3 i_Bitangent;

out VS_OUT{
	vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;

}vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    gl_Position = projection * view * model * vec4(i_Pos,1.0f);
    vs_out.FragPos = vec3(model * vec4(i_Pos,1.0));
    vs_out.TexCoords = i_Texcoords;

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * i_Tangent); // x
    vec3 B = normalize(normalMatrix * i_Bitangent); // y
    vec3 N = normalize(normalMatrix * i_Normal); // z

    mat3 TBN = transpose(mat3(T,B,N));
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos = TBN * viewPos;
    vs_out.TangentFragPos = TBN * vs_out.FragPos;


}

#type fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

out VS_OUT{

	vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;

}fs_in;

uniform sampler2D u_DiffuseMap;
uniform sampler2D u_NormalMap;

uniform bool u_NormalMapping;

void main()
{
    vec3 normal = texture(u_NormalMap,fs_in.TexCoords).rgb;

    normal = normalize(normal * 2.0 -vec3(1.0, 1.0, 1.0));

    vec3 color = texture(u_DiffuseMap,fs_in.TexCoords).rgb;

    vec3 ambient = 0.1 * color;

    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir,normal),0.0);
    vec3 diffuse = diff * color;

    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir,normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal,halfwayDir),0.0),32.0);
    vec3 specular = spec * vec3(0.2);
    
    FragColor = vec4(ambient + diffuse + specular,1.0f);

}