#type vertex
#version 430 core
layout (location = 0) in vec3 i_Pos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(i_Pos, 1.0);
}


#type fragment
#version 430 core

void main()
{             
    // gl_FragDepth = gl_FragCoord.z;
}