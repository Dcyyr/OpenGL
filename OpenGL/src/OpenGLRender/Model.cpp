//#include "Model.h"
//
//
//Model::Model(string const& path, bool gamma)
//	:gammaCorrection(gamma)
//{
//	loadModel(path);
//}
//
//void Model::DrawMode(Shader& shader)
//{
//    for (uint32_t i = 0; i < m_Meshes.size(); i++)
//        m_Meshes[i].Draw(shader);
//}

//void Model::LoadModel(std::string const& path)
//{
//	Assimp::Importer importer;
//	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
//	//¼ì²éerror
//	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
//	{
//		std::cout << "ERROR::ASSIMP::"<<importer.GetErrorString() << std::endl;
//		return;
//	}
//
//	m_Directory = path.substr(0, path.find_last_of('/'));
//
//	ProcessNode(scene->mRootNode, scene);
//}
//
//void Model::ProcessNode(aiNode* node, const aiScene* scene)
//{
//	for (uint32_t i = 0; i < node->mNumMeshes; i++)
//	{
//		// the node object only contains indices to index the actual objects in the scene. 
//		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
//		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//		m_Meshes.push_back(ProcessMesh(mesh, scene));
//	}
//	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
//	for (uint32_t i = 0; i < node->mNumChildren; i++)
//	{
//		ProcessNode(node->mChildren[i], scene);
//	}
//}
//
//Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
//{
//	std::vector<Vertex> vertices;
//	std::vector<uint32_t> indices;
//	std::vector<TextureProps> textures;
//
//    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
//    {
//        Vertex vertex;
//        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
//        // positions
//        vector.x = mesh->mVertices[i].x;
//        vector.y = mesh->mVertices[i].y;
//        vector.z = mesh->mVertices[i].z;
//        vertex.Position = vector;
//        // normals
//        if (mesh->HasNormals())
//        {
//            vector.x = mesh->mNormals[i].x;
//            vector.y = mesh->mNormals[i].y;
//            vector.z = mesh->mNormals[i].z;
//            vertex.Normal = vector;
//        }
//        // texture coordinates
//        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
//        {
//            glm::vec2 vec;
//            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
//            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
//            vec.x = mesh->mTextureCoords[0][i].x;
//            vec.y = mesh->mTextureCoords[0][i].y;
//            vertex.TexCoords = vec;
//            // tangent
//            vector.x = mesh->mTangents[i].x;
//            vector.y = mesh->mTangents[i].y;
//            vector.z = mesh->mTangents[i].z;
//            vertex.Tangent = vector;
//            // bitangent
//            vector.x = mesh->mBitangents[i].x;
//            vector.y = mesh->mBitangents[i].y;
//            vector.z = mesh->mBitangents[i].z;
//            vertex.Bitangent = vector;
//        }
//        else
//            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
//
//        vertices.push_back(vertex);
//    }
//    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
//    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
//    {
//        aiFace face = mesh->mFaces[i];
//        // retrieve all indices of the face and store them in the indices vector
//        for (unsigned int j = 0; j < face.mNumIndices; j++)
//            indices.push_back(face.mIndices[j]);
//    }
//    // process materials
//    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
//    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
//    // Same applies to other texture as the following list summarizes:
//    // diffuse: texture_diffuseN
//    // specular: texture_specularN
//    // normal: texture_normalN
//
//    // 1. diffuse maps
//    std::vector<TextureProps> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
//    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//    // 2. specular maps
//    std::vector<TextureProps> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
//    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
//    // 3. normal maps
//    std::vector<TextureProps> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
//    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
//    // 4. height maps
//    std::vector<TextureProps> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
//    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
//
//    // return a mesh object created from the extracted mesh data
//    return Mesh(vertices, indices, textures);
//}
//
//std::vector<TextureProps> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName)
//{
//	std::vector<TextureProps> textures;
//	for (uint32_t i = 0; i < material->GetTextureCount(type); i++)
//	{
//		aiString str;
//		material->GetTexture(type, i, &str);
//
//		bool skip = false;
//		for (uint32_t j = 0; j < m_LoadTextures.size(); j++)
//		{
//			if (std::strcmp(m_LoadTextures[j].Path.data(), str.C_Str()) == 0)
//			{
//				textures.push_back(m_LoadTextures[j]);
//				skip = true;
//				break;
//			}
//
//		}
//		if (!skip)
//		{
//			TextureProps texture;
//			texture.Id = TextureFromFile(str.C_Str(), this->m_Directory);
//			texture.Type = typeName;
//			texture.Path = str.C_Str();
//			textures.push_back(texture);
//			m_LoadTextures.push_back(texture);
//		}
//	}
//	return textures;
//}
//
//uint32_t TextureFromFile(const char* path, const std::string& directory,bool gamma)
//{
//    std::string filename = std::string(path);
//    filename = directory + '/' + filename;
//
//    uint32_t RendererID;
//    glGenTextures(1, &RendererID);
//
//    int width, height, nrComponents;
//    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
//    if (data)
//    {
//        GLenum format;
//        if (nrComponents == 1)
//            format = GL_RED;
//        else if (nrComponents == 3)
//            format = GL_RGB;
//        else if (nrComponents == 4)
//            format = GL_RGBA;
//
//        glBindTexture(GL_TEXTURE_2D, RendererID);
//        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        stbi_image_free(data);
//    }
//    else
//    {
//        std::cout << "Texture failed to load at path: " << path << std::endl;
//        stbi_image_free(data);
//    }
//
//    return RendererID;
//}
