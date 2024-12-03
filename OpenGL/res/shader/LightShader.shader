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

struct Materials
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;//反光度
};

struct Light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 position;
};

in vec3 Normal;
in vec3 FragPos;

uniform Materials materials;
uniform Light light;


uniform vec3 LightPos;
uniform vec3 ViewPos;


void main()
{	
	//环境光照
	vec3 ambient = materials.ambient * light.ambient;

	//漫反射
	vec3 normal = normalize(Normal);//向量标准化must be
	vec3 lightDir = normalize(LightPos - FragPos);
	float diff = max(dot(normal,lightDir),0.0);
	vec3 diffuse = (diff*materials.diffuse) * light.diffuse;

	//镜面反射
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular =  (spec* materials.specular) * light.specular;  

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result,1.0f);
}