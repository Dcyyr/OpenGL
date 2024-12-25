#pragma once

#include <OpenGLRender/shader.h>

#include <string>
#include <vector>

const int MAX_BONE_INFLUENCE = 4;

struct VertexProps 
{
    // 位置
    glm::vec3 Position;
    // 法线
    glm::vec3 Normal;
    // 纹理坐标
    glm::vec2 TexCoords;
    // 切线
    glm::vec3 Tangent;
    // 位切线
    glm::vec3 Bitangent;
    //影响该顶点的骨索引
    int BoneIDs[MAX_BONE_INFLUENCE];
    //每块骨头的重量
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
    // 初始化所有缓冲区对象/数组
    void SetupMesh();

private:
    // mesh Data
    std::vector<VertexProps>       m_Vertices;
    std::vector<uint32_t>          m_Indices;
    std::vector<TextureProps>      m_Textures;
    // render data 
    uint32_t VBO, EBO, VAO;
   
    
};