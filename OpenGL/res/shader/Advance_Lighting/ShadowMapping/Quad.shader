#type vertex
#version 430 core
layout (location = 0) in vec3 i_Pos;
layout (location = 1) in vec2 i_TexCoords;

out vec2 o_TexCoords;

void main()
{
    o_TexCoords = i_TexCoords;
    gl_Position = vec4(i_Pos, 1.0);
}


#type fragment
#version 430 core
out vec4 FragColor;

in vec2 o_TexCoords;

uniform sampler2D depthMap;
uniform float near_plane;
uniform float far_plane;

// 使用透视投影矩阵时需要
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

void main()
{             
    float depthValue = texture(depthMap, o_TexCoords).r;
    //FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspective
    FragColor = vec4(vec3(depthValue), 1.0); // orthographic
}
