#type vertex
#version 330 core
layout (location = 0) in vec4 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	FragPos = vec3(model* a_Pos);
	Normal = mat3(transpose(inverse(model))) * a_Normal;
	gl_Position =projection * view *vec4(FragPos,1.0);

	TexCoords = a_TexCoord;
}


#type fragment
#version 330 core

layout(location = 0)out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light {
    vec3 direction;//定向光
    vec3 position;
    float cutOff;//切光角
    float outerCutOff;//外光切


    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;//常数
    float linear;//线性
    float quadratic;//二次
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // 环境光反射
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  	
    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // 镜面光反射
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
        
    //聚光（平滑边缘）I=θ?γ/?
    float theta = dot(lightDir,normalize(-light.direction));//点乘得到θ余弦值
    float epsilon = (light.cutOff - light.outerCutOff);//这里?(Epsilon)是内（?和外圆锥（γ之间的余弦值差?=??γ

    float intensity = clamp((theta - light.outerCutOff)/epsilon,0.0,1.0);//将会给它计算出一个0.0到1.0之间的强度值。
    //如果片段在内圆锥之内，它的强度就是1.0，如果在外圆锥之外强度值就是0.0，clamp就是比较函数
    diffuse *= intensity;
    specular *= intensity;

    //衰减
    float distance = length(light.position - FragPos);//片段距光源的距离
    float attenuation = 1.0/(light.constant+ light.linear *distance + light.quadratic * (distance* distance));//衰减值公式

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
} 