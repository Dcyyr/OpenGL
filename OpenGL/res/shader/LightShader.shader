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
    float shininess;//�����Խ�ߣ�����������Խǿ��ɢ���Խ�٣��߹��ͻ�ԽС
}; 

struct DirLight {

    vec3 direction;//�����

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

  
};

struct PointLight
{
    vec3 position;

    float constant;//����
    float linear;//����
    float quadratic;//����
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};

struct SpotLight
{
    vec3 position;
    vec3 direction;

    float cutOff;//�й��
    float outerCutOff;//�����

    float constant;//����
    float linear;//����
    float quadratic;//����

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
    //������
    float diff = max(dot(normal, lightDir), 0.0);//�����������֮��ĽǶȴ���90�ȣ���˵Ľ���ͻ��ɸ����������ᵼ�������������Ϊ������
    //Ϊ�ˣ�����ʹ��max����������������֮��ϴ�Ĳ������Ӷ���֤��������������ɸ�����������ɫ�Ĺ�����û�ж����
    //����
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//�����ȼ������߷����뷴�䷽��ĵ�ˣ���ȷ�������Ǹ�ֵ����
    //Ȼ��ȡ����32����

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
    
    float distance = length(light.position - fragPos);//Ƭ�ξ��Դ�ľ���
    float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic *(distance * distance));//˥��ֵ��ʽ  
    
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
   
  //�۹⣨ƽ����Ե��I=��?��/?
    float theta = dot(lightDir,normalize(-light.direction));//��˵õ�������ֵ
    float epsilon = (light.cutOff - light.outerCutOff);//����?(Epsilon)���ڣ�?����Բ׶����֮�������ֵ��?=??��
    float intensity = clamp((theta - light.outerCutOff)/epsilon,0.0,1.0);//������������һ��0.0��1.0֮���ǿ��ֵ��
    //���Ƭ������Բ׶֮�ڣ�����ǿ�Ⱦ���1.0���������Բ׶֮��ǿ��ֵ����0.0��clamp���ǱȽϺ���
    
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}


