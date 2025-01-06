#type vertex
#version 330 core
layout (location = 0) in vec3 i_Pos;

out vec3 o_TexCoords;
//未传入model矩阵是因为盒子的中心永远是世界中心
uniform mat4 projection;
uniform mat4 view;

void main()
{
    o_TexCoords = vec3(i_Pos.x, -i_Pos.y, i_Pos.z);//因为盒子中心与立方体纹理中心都在世界坐标原点，所以使用顶点位置的插值进行采样
    vec4 pos = projection * view * vec4(i_Pos, 1.0);
    gl_Position = pos.xyww;
}

#type fragment
#version 330 core
out vec4 FragColor;

in vec3 o_TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, o_TexCoords);
}