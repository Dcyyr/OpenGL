#pragma once
#include <iostream>
#include <glad/glad.h>
#include "Core/Debug.h"
class Texture2D
{
public:
	Texture2D(uint32_t width, uint32_t height);
	Texture2D(const std::string& filepath);
	virtual ~Texture2D();

	virtual uint32_t GetWidth() const { return m_Width; }
	virtual uint32_t GetHeight() const { return m_Height; }

	void SetData(void* data, uint32_t size);
	virtual void Bind(uint32_t slot =0) const;

private:
	uint32_t m_Width, m_Height;
	uint32_t m_RendererID;
	std::string m_Filepath;
};