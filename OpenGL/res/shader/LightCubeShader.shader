#type vertex
#version 330 core
layout (location = 0) in vec4 a_Pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	gl_Position = projection * view * model * a_Pos;
}

#type fragment
#version 330 core
layout(location =0) out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0); // set all 4 vector values to 1.0
}