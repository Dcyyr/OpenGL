#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures)
{
	this->m_vertices = vertices;
	this->m_indices = indices;
	this->m_textures = textures;

	SetupMesh();
}

void Mesh::Draw(Shader &shader)
{
	uint32_t DiffuseN  = 1;
	uint32_t SpecularN = 1;
	uint32_t NormalN   = 1;
	uint32_t HeightN   = 1;

	for (uint32_t i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		std::string Number;
		std::string name = m_textures[i].Type;

		if (name == "texture_diffuse")
			Number = std::to_string(DiffuseN++);
		else if (name == "texture_specular")
			Number = std::to_string(SpecularN++);
		else if (name == "texture_normal")
			Number = std::to_string(NormalN++);
		else if (name == "texture_height")
			Number = std::to_string(HeightN++);

		glUniform1i(glGetUniformLocation(m_RendererID, (name + Number).c_str()), i);

		glBindTexture(GL_TEXTURE_2D, m_textures[i].Id);


	}
	// draw mesh
	glBindVertexArray(m_RendererID);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//一旦配置完成，将所有内容设置回默认值总是好的做法
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::SetupMesh()
{
	glCreateVertexArrays(1,&m_RendererID);
	glCreateBuffers(1, &m_RendererID);
	glCreateBuffers(1, &m_RendererID);
	
	glBindVertexArray(m_RendererID);

	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER,m_vertices.size() * sizeof(m_vertices),&m_vertices[0],GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint32_t), &m_indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	glEnableVertexAttribArray(5);
	glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, BoneIDs));

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Weights));

	glBindVertexArray(0);
}