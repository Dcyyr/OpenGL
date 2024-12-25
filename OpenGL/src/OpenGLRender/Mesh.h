#pragma once

#include <OpenGLRender/shader.h>

#include <string>
#include <vector>

const int MAX_BONE_INFLUENCE = 4;

struct VertexProps 
{
    // λ��
    glm::vec3 Position;
    // ����
    glm::vec3 Normal;
    // ��������
    glm::vec2 TexCoords;
    // ����
    glm::vec3 Tangent;
    // λ����
    glm::vec3 Bitangent;
    //Ӱ��ö���Ĺ�����
    int BoneIDs[MAX_BONE_INFLUENCE];
    //ÿ���ͷ������
    float Weights[MAX_BONE_INFLUENCE];
};

struct TextureProps 
{
    unsigned int Id;
    std::string Type;
    std::string Path;
};

class Mesh 
{
public:
    Mesh(std::vector<VertexProps> vertices, std::vector<unsigned int> indices, std::vector<TextureProps> textures);
 
    // render the mesh
    void Draw(Shader& shader);

private:
    // ��ʼ�����л���������/����
    void SetupMesh();

private:
    // mesh Data
    std::vector<VertexProps>       m_Vertices;
    std::vector<uint32_t>          m_Indices;
    std::vector<TextureProps>      m_Textures;
    // render data 
    uint32_t VBO, EBO, VAO;
   
    
};