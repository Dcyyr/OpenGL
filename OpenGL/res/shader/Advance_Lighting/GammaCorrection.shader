//不要多出空格 :(
#type vertex
#version 430 core
layout (location = 0) in vec3 i_Pos;
layout (location = 1) in vec3 i_Normal;
layout (location = 2) in vec2 i_TexCoords;


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

uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];
uniform vec3 viewPos;
uniform bool gamma;

vec3 BlinnPhong(vec3 normal,vec3 fragPos,vec3 lightPos,vec3 lightColor)
{
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(lightDir,normal),0.0);
    vec3 diffuse = diff * lightColor;
    //镜面
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal,halfwayDir),0.0),64.0);
    vec3 specular = spec * lightColor;
    // attenuation
    float max_distance = 1.5;
    float distance = length(lightPos - fragPos);
    float attenuation = 1.0/(gamma ? distance *distance : distance);

    diffuse  *= attenuation;
    specular *= attenuation;
    return diffuse + specular;
}

void main()
{           
    vec3 color = texture(texture1, fs_in.TexCoords).rgb;
    vec3 lighting = vec3(0.0);
    for(int i =0;i<4;i++)
        lighting +=BlinnPhong(normalize(fs_in.Normal),fs_in.FragPos,lightPositions[i],lightColors[i]);
    color *= lighting;
    if(gamma)
        color = pow(color,vec3(1.0/2.2));
    FragColor = vec4(color,1.0);
       
   
}