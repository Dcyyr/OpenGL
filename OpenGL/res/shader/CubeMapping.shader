#type vertex
#version 330 core
layout (location = 0) in vec4 i_Pos;
layout (location = 1) in vec3 i_Normal;

out vec3 o_Normal;
out vec3 o_Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    o_Normal = mat3(transpose(inverse(model))) * i_Normal;
    o_Position = vec3(model * i_Pos);
    gl_Position = projection * view * model * i_Pos;
}

#type fragment
#version 330 core
layout(location = 0)out vec4 FragColor;

in vec3 o_Normal;
in vec3 o_Position;


uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{    
    float ratio = 1.00-1.52;//±»¬ 
    vec3 I = normalize(o_Position - cameraPos);
    // vec3 R = reflect(I,normalize(o_Normal));∑¥…‰
    vec3 R = refract(I,normalize(o_Normal),ratio);//’€…‰
    FragColor =vec4(texture(skybox, R).rgb,1.0);
}