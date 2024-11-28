#include "Renderer.h"


void Renderer::Draw(const VertexBuffer& vb, const IndexBuffer& ib, const Shader& shader)const
{
	vb.Bind();
	ib.Bind();
	shader.Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear() const
{
	glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
