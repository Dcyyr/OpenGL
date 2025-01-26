#pragma once
#include "Buffers.h"

struct FrameBufferSpecification
{
	uint32_t width, height;
	uint32_t ShadowWidth, ShadowHeight;

};

class FrameBuffers
{
public:
	FrameBuffers(const FrameBufferSpecification& spec);
	~FrameBuffers();

	void Invalidate();

	void Bind() const;
	void Unbind() const;

	void BindTextures(uint32_t id =0 );

	void Resize(uint32_t width, uint32_t height);

	uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; }
	const FrameBufferSpecification& GetSpecification() const { return m_Specification; }

private:
	uint32_t m_RendererID;
	uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
	FrameBufferSpecification m_Specification;

};