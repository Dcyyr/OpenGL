#include "Shader.h"
#include <fstream>
#include <array>

static GLenum ShaderTypeFromString(const std::string& type)
{
	if (type == "vertex")
		return GL_VERTEX_SHADER;
	if (type == "fragment" || type == "pixel")
		return GL_FRAGMENT_SHADER;
	assert(false, "Unknown shader type");
	return 0;
}

Shader::Shader(const std::string& filepath)
{
	std::string source = ReadFile(filepath);
	auto shaderSources = PreProcess(source);
	Compile(shaderSources);

}

Shader::Shader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
	:m_Name(name)
{
	
	std::unordered_map<GLenum, std::string> sources;
	sources[GL_VERTEX_SHADER] = vertexSource;
	sources[GL_FRAGMENT_SHADER] = fragmentSource;
	Compile(sources);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

std::string Shader::ReadFile(const std::string& filepath)
{
	std::string result;
	std::ifstream in(filepath, std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);//�ҵ��ļ�ĩβ
		result.resize(in.tellg());//�������ļ���ָ��λ�����������֪���ļ��Ĵ�С
		in.seekg(0, std::ios::beg);//�ص��ʼ
		in.read(&result[0], result.size());//��ͷд��
	}
	else
	{
		assert("�򲻿��ļ�'{0}'", filepath);
	}

	return result;

}

std::unordered_map<GLenum, std::string> Shader::PreProcess(const std::string& source)
{
	std::unordered_map<GLenum, std::string> shaderSource;

	const char* typeToken = "#type";//ָ�򲻿ɱ���ַ�
	size_t typeTokenLength = strlen(typeToken);//size_t ���޴�
	size_t pos = source.find(typeToken, 0);//���±�0��ʼ���ҵ�һ�γ��ֵ�Ŀ���ַ���

	while (pos != std::string::npos)//ֱ���ַ�����β
	{
		size_t eol = source.find_first_of("\r\n", pos);//�������Ҫ���е��ַ����ҵ������±꣬��pos�±꿪ʼ
		size_t begin = pos + typeTokenLength + 1;//typeTokenLength +1 ����glsl�ļ����#type��ͷ���Ͽո� //example #type vertex

		std::string type = source.substr(begin, eol - begin);//��ͷ��ʼ������Ϊ ĩβ-��ʼ

		size_t nextLinePos = source.find_first_not_of("\r\n", eol);//��eol��ʼ��������ǻ����ַ��������±�
		pos = source.find(typeToken, nextLinePos);
		
		shaderSource[ShaderTypeFromString(type)] = 
			source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
	}

	return shaderSource;
}

void Shader::Compile(std::unordered_map<GLenum,std::string>& shaderSources)
{
	
	GLuint program = glCreateProgram();
	std::array<GLenum, 2> glShaderIDs;
	int glShaderIDIndex = 0;
	for (auto& kv : shaderSources)
	{
		GLenum type = kv.first;
		const std::string& source = kv.second;

		GLuint shader = glCreateShader(type);

		const GLchar* sourceCStr = source.c_str();
		GLCall(glShaderSource(shader, 1, &sourceCStr, 0));

		GLCall(glCompileShader(shader));

		GLint isCompiled = 0;
		GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled));
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			GLCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength));

			std::vector<GLchar> infoLog(maxLength);
			GLCall(glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]));

			GLCall(glDeleteShader(shader));

			break;
		}

		GLCall(glAttachShader(program, shader));
		glShaderIDs[glShaderIDIndex++] = shader;
	}

	m_RendererID = program;

	// Link our program
	GLCall(glLinkProgram(program));

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	GLCall(glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked));
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength));

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		GLCall(glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]));

		// We don't need the program anymore.
		GLCall(glDeleteProgram(program));

		for (auto id : glShaderIDs)
			GLCall(glDeleteShader(id));

		return;
	}

	for (auto id : glShaderIDs)
		GLCall(glDetachShader(program, id));
	

}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetInt(const std::string& name,int value)
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());//��ȡȫ�ֱ���uniform��glsl���ֵ��������
	glUniform1i(location, value); //����uniformֵ
	//��ѯuniform��ַ��Ҫ����֮ǰʹ�ù���ɫ�����򣬵��Ǹ���һ��uniform֮ǰ�������ʹ�ó��򣨵���glUseProgram)
	//��Ϊ�����ڵ�ǰ�������ɫ������������uniform�ġ�
}

void Shader::SetFloat(const std::string& name, float value)
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform1f(location, value);
}

void Shader::SetFloat3(const std::string& name, const glm::vec3& value)
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform3f(location, value.x, value.y,value.z);
}

void Shader::SetFloat4(const std::string& name, const glm::vec4& value)
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform4f(location, value.x, value.y, value.z,value.w);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& matrix)
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetBool(const std::string& name, bool value)
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform1i(location, (int)value);
}

