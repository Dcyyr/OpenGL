#pragma once
#include <iostream>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Debug.h"

typedef unsigned int GLenum;

class Shader
{
public:
	Shader(const std::string& filepath);
	Shader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
	Shader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometryshader);
	virtual ~Shader();

	virtual void Bind() const;
	virtual void Unbind() const;

	virtual void SetUniformInt(const std::string& name, int value) const ;
	virtual void SetUniformFloat(const std::string& name, float value)const;
	virtual void SetUniformFloat3(const std::string& name, const glm::vec3& value)const;
	virtual void SetUniformFloat4(const std::string& name, const glm::vec4& value)const;
	virtual void SetUniformMat4(const std::string& name, const glm::mat4& value)const;
	virtual void SetUniformBool(const std::string& name, bool value)const;
private:
	std::string ReadFile(const std::string& filepath);
	std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
	void Compile(std::unordered_map<GLenum, std::string>& shadersource);
public:
	uint32_t m_RendererID;
	std::string m_Name;
};