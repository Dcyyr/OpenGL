#include "Texture2D.h"

#include "stb_image.h"

Texture2D::Texture2D(const std::string& filepath)
	:m_Filepath(filepath)
{
	stbi_set_flip_vertically_on_load(1);
	int width, height;
	stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, nullptr, 4);
	GLCall(glCreateTextures(GL_TEXTURE_2D,1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTextureParameteri(m_RendererID,GL_TEXTURE_WRAP_T,GL_REPEAT));
	GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT));

	GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST));

	if (data)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));

		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else
	{
		std::cout << "¼ÓÔØÍ¼Æ¬Ê§°Ü" << std::endl;
	}

	stbi_image_free(data);

}

Texture2D::~Texture2D()
{
	GLCall(glDeleteTextures(1,&m_RendererID));
}


void Texture2D::Bind(uint32_t slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}
