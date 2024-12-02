#type vertex
#version 330 core
layout (location = 0) in vec4 a_Pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position =projection * view * model * a_Pos;
}


#type fragment
#version 330 core
layout (location =0) out vec4 FragColor;

uniform vec3 LightColor;
uniform vec3 ObjectColor;

void main()
{
	FragColor = vec4(ObjectColor*LightColor,1.0f);
}