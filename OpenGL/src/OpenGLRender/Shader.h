#pragma once
#include <iostream>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Debug.h"

typedef unsigned int GLenum;

class Shader
{
public:
	Shader(const std::string& filepath);
	Shader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
	virtual ~Shader();

	virtual void Bind() const;
	virtual void Unbind() const;

	virtual void SetInt(const std::string& name, int value);
	virtual void SetFloat(const std::string& name, float value);
	virtual void SetFloat3(const std::string& name, const glm::vec3& value);
	virtual void SetFloat4(const std::string& name, const glm::vec4& value);
	virtual void SetMat4(const std::string& name, const glm::mat4& value);
	virtual void SetBool(const std::string& name, bool value);
private:
	std::string ReadFile(const std::string& filepath);
	std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
	void Compile(std::unordered_map<GLenum, std::string>& shadersource);
private:
	uint32_t m_RendererID;
	std::string m_Name;
};