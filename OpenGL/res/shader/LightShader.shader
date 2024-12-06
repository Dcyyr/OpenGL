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
    vec3 direction;//�����
    vec3 position;
    float cutOff;//�й��
    float outerCutOff;//�����


    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;//����
    float linear;//����
    float quadratic;//����
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // �����ⷴ��
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  	
    // ������
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // ����ⷴ��
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
        
    //�۹⣨ƽ����Ե��I=��?��/?
    float theta = dot(lightDir,normalize(-light.direction));//��˵õ�������ֵ
    float epsilon = (light.cutOff - light.outerCutOff);//����?(Epsilon)���ڣ�?����Բ׶����֮�������ֵ��?=??��

    float intensity = clamp((theta - light.outerCutOff)/epsilon,0.0,1.0);//������������һ��0.0��1.0֮���ǿ��ֵ��
    //���Ƭ������Բ׶֮�ڣ�����ǿ�Ⱦ���1.0���������Բ׶֮��ǿ��ֵ����0.0��clamp���ǱȽϺ���
    diffuse *= intensity;
    specular *= intensity;

    //˥��
    float distance = length(light.position - FragPos);//Ƭ�ξ��Դ�ľ���
    float attenuation = 1.0/(light.constant+ light.linear *distance + light.quadratic * (distance* distance));//˥��ֵ��ʽ

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
} 