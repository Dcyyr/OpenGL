#type vertex
#version 330 core
layout (location = 0) in vec4 a_Pos;
layout (location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

uniform mat4 transform;

void main()
{
    gl_Position = transform* a_Pos;
    v_TexCoord = a_TexCoord;
    
}


#type fragment
#version 330 core
layout (location = 0) out vec4 FragColor;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    vec4 texcolor = texture(u_Texture,v_TexCoord);
    FragColor = texcolor;
}