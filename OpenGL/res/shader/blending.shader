#type vertex
#version 330 core
layout (location = 0) in vec4 a_Pos;
layout (location = 1) in vec2 a_TexCoords;

out vec2 TexCoords;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = a_TexCoords;    
    gl_Position = projection * view * model * a_Pos;
}


#type fragment
#version 330 core

layout(location = 0)out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{
	FragColor = texture(texture1,TexCoords);
}