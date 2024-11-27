#pragma once

#include "Buffers.h"
#include "Shader.h"

class Renderer
{
public:
	void Draw(const VertexBuffer& vb, const IndexBuffer& ib, const Shader& shader) const;
	void Clear() const;
private:

};
