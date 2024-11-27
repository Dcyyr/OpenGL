#pragma once
#include <iostream>	
#include "Core/Debug.h"
#include "Buffers.h"
#include "VertexBufferLayout.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void AddVertexBuffer(const VertexBuffer& vb,const VertexBufferLayout& layout);
	void Bind() const;
	void Unbind() const;

private:
	uint32_t m_RendererID;
};