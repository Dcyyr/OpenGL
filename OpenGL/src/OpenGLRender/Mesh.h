#pragma once

#include <OpenGLRender/shader.h>

#include <string>
#include <vector>

const int MAX_BONE_INFLUENCE = 4;

struct VertexProps 
{
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
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
    // constructor
    Mesh(std::vector<VertexProps> vertices, std::vector<unsigned int> indices, std::vector<TextureProps> textures);
 
    // render the mesh
    void Draw(Shader& shader);

private:
    // initializes all the buffer objects/arrays
    void SetupMesh();

private:
    // mesh Data
    std::vector<VertexProps>       m_Vertices;
    std::vector<uint32_t>          m_Indices;
    std::vector<TextureProps>      m_Textures;
    // render data 
    uint32_t VBO, EBO, VAO;
   
    
};