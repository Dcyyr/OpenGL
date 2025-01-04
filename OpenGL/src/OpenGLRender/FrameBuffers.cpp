#include "FrameBuffers.h"

static const uint32_t s_MaxFramebufferSize = 8192;


FrameBuffers::FrameBuffers(const FrameBufferSpecification& spec)
	:m_Specification(spec)
{
	Invalidate();
}

FrameBuffers::~FrameBuffers()
{
	glDeleteFramebuffers(1, &m_RendererID);
	glDeleteTextures(1, &m_ColorAttachment);
	glDeleteTextures(1, &m_DepthAttachment);
}

void FrameBuffers::Invalidate()
{
	glCreateFramebuffers(1, &m_RendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
	glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
	glTexImage2D(GL_TEXTURE_2D, 1, GL_RGBA, m_Specification.width, m_Specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

	/*glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
	glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.width, m_Specification.height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);*/

	glGenRenderbuffers(1, &m_DepthAttachment);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachment);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Specification.width, m_Specification.height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachment);

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "帧缓冲器无法使用");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void FrameBuffers::BindTextures(uint32_t id)
{
	glBindTexture(GL_TEXTURE_2D,id);
}

void FrameBuffers::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	//glViewport(0, 0, m_Specification.width, m_Specification.height);
}

void FrameBuffers::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffers::Resize(uint32_t width, uint32_t height)
{
	if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
	{
		ASSERT("试图将帧缓冲区的大小调整为{0}，{1}", width, height);
		return;
	}

	m_Specification.width = width;
	m_Specification.height = height;

	Invalidate();
}