#pragma once
#include <iostream>
#include <glad/glad.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "OpenGLRender/Shader.h"

const int MAX_BONE_INFLUENCE = 4;

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

	glm::vec3 Tangent;//切线

	glm::vec3 Bitangent;//位切线
	//骨索引
	int BoneIDs[MAX_BONE_INFLUENCE];
	//每根骨头的重量
	float Weights[MAX_BONE_INFLUENCE];

};

struct Texture
{
	uint32_t Id;
	std::string Path;
	std::string Type;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices,std::vector<uint32_t> indices,std::vector<Texture> textures);

	void Draw(Shader& shader);

private:
	void SetupMesh();
private:
	uint32_t m_RendererID;

	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;
	std::vector<Texture> m_textures;
};