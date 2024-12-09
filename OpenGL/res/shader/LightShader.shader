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
out vec4 FragColor;

struct Materials {

    sampler2D diffuse;
    sampler2D specular;    
    float shininess;//反光度越高，反射光的能力越强，散射得越少，高光点就会越小
}; 

struct DirLight {

    vec3 direction;//定向光

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

  
};

struct PointLight
{
    vec3 position;

    float constant;//常数
    float linear;//线性
    float quadratic;//二次
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};

struct SpotLight
{
    vec3 position;
    vec3 direction;

    float cutOff;//切光角
    float outerCutOff;//外光切

    float constant;//常数
    float linear;//线性
    float quadratic;//二次

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Materials material;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    

    vec3 result = CalcDirLight(dirLight, norm, viewDir);
   
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    

    }
    
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
    FragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    //漫反射
    float diff = max(dot(normal, lightDir), 0.0);//如果两个向量之间的角度大于90度，点乘的结果就会变成负数，这样会导致漫反射分量变为负数。
    //为此，我们使用max函数返回两个参数之间较大的参数，从而保证漫反射分量不会变成负数。负数颜色的光照是没有定义的
    //镜面
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//我们先计算视线方向与反射方向的点乘（并确保它不是负值），
    //然后取它的32次幂

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    float diff = max(dot(normal, lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    float distance = length(light.position - fragPos);//片段距光源的距离
    float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic *(distance * distance));//衰减值公式  
    
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    float diff = max(dot(normal, lightDir), 0.0);
   
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
   
  //聚光（平滑边缘）I=θ?γ/?
    float theta = dot(lightDir,normalize(-light.direction));//点乘得到θ余弦值
    float epsilon = (light.cutOff - light.outerCutOff);//这里?(Epsilon)是内（?和外圆锥（γ之间的余弦值差?=??γ
    float intensity = clamp((theta - light.outerCutOff)/epsilon,0.0,1.0);//将会给它计算出一个0.0到1.0之间的强度值。
    //如果片段在内圆锥之内，它的强度就是1.0，如果在外圆锥之外强度值就是0.0，clamp就是比较函数
    
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}


