#type vertex
#version 330 core

layout(location = 0) in vec2 a_Pos;
layout(location = 1) in vec2 a_TexCoords;

out vec2 v_TexCoords;


void main()
{
	v_TexCoords = a_TexCoords;
	gl_Position = vec4(a_Pos.x,a_Pos.y,0.0,1.0);
}

#type fragment
#version 330 core
layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoords;

uniform sampler2D ScreenTexture;

const float offset = 1.0/500;

void main()
{
	 vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(ScreenTexture, v_TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    FragColor = vec4(col, 1.0);
	//FragColor = vec4(vec3(1.0 - texture(ScreenTexture, v_TexCoords)), 1.0);//反相 也就是负片
}