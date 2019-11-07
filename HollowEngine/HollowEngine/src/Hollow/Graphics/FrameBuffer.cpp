#include <hollowpch.h>
#include "FrameBuffer.h"
#include "Utils/GLCall.h"

namespace Hollow {

	FrameBuffer::FrameBuffer(int width, int height, int Texcount, bool attachDepth) : mWidth(width), mHeight(height), mTexCount(Texcount), mpTextureID(nullptr)
	{
		GLCall(glGenFramebuffers(1, &mRendererID));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mRendererID));

		mpTextureID = new unsigned int[mTexCount];
		unsigned int* attachments = new unsigned int[mTexCount];
	
		// create floating point color buffers
		glGenTextures(mTexCount, mpTextureID);
		for (unsigned int i = 0; i < mTexCount; i++)
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, mpTextureID[i]));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, mWidth, mHeight, 0, GL_RGBA, GL_FLOAT, 0));
		
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
			//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
			// TESTING -------
			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

			// attach texture to framebuffer
			GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mpTextureID[i], 0));
			attachments[i] = GL_COLOR_ATTACHMENT0 + i;
		}

		// create and attach depth buffer (renderbuffer)
		if (attachDepth)
		{
			GLCall(glGenRenderbuffers(1, &mDepthBuffer));
			GLCall(glBindRenderbuffer(GL_RENDERBUFFER, mDepthBuffer));
			GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mWidth, mHeight));
			GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBuffer));
		}

		// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
		GLCall(glDrawBuffers(mTexCount, attachments));

		// finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{

			HW_CORE_ERROR("Error building FrameBuffer");
		}

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		delete[] attachments;
	}

	FrameBuffer::~FrameBuffer()
	{
		delete[] mpTextureID;
		Delete();
	}

	void FrameBuffer::Bind()
	{
		GLint* data = new GLint[4];
		GLCall(glGetIntegerv(GL_VIEWPORT, data));
		mScreenPosition.x = data[0];
		mScreenPosition.y = data[1];
		mScreenViewPort.x = data[2];
		mScreenViewPort.y = data[3];
		GLCall(glViewport(0, 0,mWidth, mHeight));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mRendererID));
		Clear();
	}

	void FrameBuffer::BindRead()
	{
		GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, mRendererID));
	}

	void FrameBuffer::Unbind() const
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		GLCall(glViewport(mScreenPosition.x, mScreenPosition.y, mScreenViewPort.x, mScreenViewPort.y));
		//Clear();
	}

	void FrameBuffer::TexBind(unsigned int index, unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, mpTextureID[index]);
	}

	void FrameBuffer::TexUnbind(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void FrameBuffer::Clear() const
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void FrameBuffer::Delete() const
	{
		GLCall(glDeleteFramebuffers(1, &mRendererID));
	}

	void FrameBuffer::ChangeSize(unsigned int width, unsigned int height)
	{
		mHeight = height;
		mWidth = width;
		for (unsigned int i = 0; i < mTexCount; i++)
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, mpTextureID[i]));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, mWidth, mHeight, 0, GL_RGB, GL_FLOAT, 0));

			GLCall(glBindRenderbuffer(GL_RENDERBUFFER, mDepthBuffer));
			GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mWidth, mHeight));
		}
	}

}