#type vertex
#version 330 core
layout (location = 0) in vec4 a_Pos;
layout (location = 1) in vec4 a_Color;

void main()
{
    gl_Position = a_Pos;
    
}


#type fragment
#version 330 core
layout (location = 0) out vec4 FragColor;

uniform vec4 u_Color;

void main()
{
    FragColor = u_Color;
}



