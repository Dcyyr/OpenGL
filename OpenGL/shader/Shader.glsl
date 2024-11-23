#type vertex
#version 330 core
layout (location = 0) in vec3 a_Pos;   // λ�ñ���������λ��ֵΪ 0 
layout (location = 1) in vec3 a_Color; // ��ɫ����������λ��ֵΪ 1

out vec3 ourColor; // ��Ƭ����ɫ�����һ����ɫ

void main()
{
    gl_Position = vec4(a_Pos, 1.0);
    ourColor = a_Color; // ��ourColor����Ϊ���ǴӶ�����������õ���������ɫ
}


#type fragment
#version 330 core
out vec4 FragColor;  
in vec3 ourColor;

void main()
{
    FragColor = vec4(ourColor, 1.0);
}