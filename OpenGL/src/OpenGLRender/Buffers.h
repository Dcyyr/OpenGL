#pragma once

#include <iostream>
#include "glad/glad.h"
#include "Core/Debug.h"

class VertexBuffer
{
public:
	VertexBuffer(float* vertices,uint32_t size);
	~VertexBuffer();

	virtual void Bind() const;
	virtual void Unbind() const;
private:
	uint32_t m_RendererID;
};




class IndexBuffer
{
public:
	IndexBuffer(uint32_t* indices, uint32_t count);
	~IndexBuffer();

	virtual void Bind() const;
	virtual void Unbind() const;

private:
	uint32_t m_RendererID;
	uint32_t m_Count;
};
