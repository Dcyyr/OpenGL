#type vertex
#version 330 core
layout (location = 0) in vec3 a_Pos;   // 位置变量的属性位置值为 0 
layout (location = 1) in vec3 a_Color; // 颜色变量的属性位置值为 1

out vec3 ourColor; // 向片段着色器输出一个颜色

void main()
{
    gl_Position = vec4(a_Pos, 1.0);
    ourColor = a_Color; // 将ourColor设置为我们从顶点数据那里得到的输入颜色
}


#type fragment
#version 330 core
out vec4 FragColor;  
in vec3 ourColor;

void main()
{
    FragColor = vec4(ourColor, 1.0);
}