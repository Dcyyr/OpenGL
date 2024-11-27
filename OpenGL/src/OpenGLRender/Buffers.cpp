#include "Buffers.h"

//vertex
VertexBuffer::VertexBuffer(float* vertices, uint32_t size)
{
	GLCall(glCreateBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER,size,vertices,GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1,&m_RendererID));
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

}

//index
IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count)
	:m_Count(count)
{
	GLCall(glCreateBuffers(1,&m_RendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_RendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER,count*sizeof(uint32_t), indices, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1,&m_RendererID));
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

}