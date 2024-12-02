#type vertex
#version 330 core
layout (location = 0) in vec4 a_Pos;
layout (location = 1) in vec3 a_Normal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	FragPos = vec3(model* a_Pos);
	Normal = mat3(transpose(inverse(model))) * a_Normal;
	gl_Position =projection * view *vec4(FragPos,1.0);
}


#type fragment
#version 330 core
layout (location =0) out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 LightPos;
uniform vec3 ViewPos;


uniform vec3 LightColor;
uniform vec3 ObjectColor;

void main()
{	
	//��������
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * LightColor;

	//������
	vec3 normal = normalize(Normal);//������׼��must be
	vec3 lightDir = normalize(LightPos - FragPos);
	float diff = max(dot(normal,lightDir),0.0);
	vec3 diffuse = diff * LightColor;

	//���淴��
	float specularStrength = 0.5;
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * LightColor;  

	vec3 result = (ambient + diffuse + specular) * ObjectColor;
	FragColor = vec4(result,1.0f);
}