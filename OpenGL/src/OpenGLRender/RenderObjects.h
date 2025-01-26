#pragma once

#include "Buffers.h"
#include "Shader.h"

class Renderer
{
public:
	//for now
	void RenderScene(const Shader& shader);
	void RenderCube();
	void RenderQuad();
	void RenderPlane();

	void Draw(const VertexBuffer& vb, const IndexBuffer& ib, const Shader& shader) const;
	void Clear() const;
private:
	uint32_t m_RendererID = 0;
	uint32_t m_CubeVa = 0;
	uint32_t m_QuadVa = 0;
	uint32_t m_PlaneVa = 0;
};
