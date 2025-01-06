#type vertex
#version 330 core
layout (location = 0) in vec3 i_Pos;

out vec3 o_TexCoords;
//δ����model��������Ϊ���ӵ�������Զ����������
uniform mat4 projection;
uniform mat4 view;

void main()
{
    o_TexCoords = vec3(i_Pos.x, -i_Pos.y, i_Pos.z);//��Ϊ�����������������������Ķ�����������ԭ�㣬����ʹ�ö���λ�õĲ�ֵ���в���
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