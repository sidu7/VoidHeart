/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#include <hollowpch.h>
#include "Utils/GLCall.h"
#include "VertexBuffer.h"

namespace Hollow {

	VertexBuffer::VertexBuffer()
	{
		GLCall(glGenBuffers(1, &mRendererID));
	}

	VertexBuffer::~VertexBuffer()
	{
		GLCall(glDeleteBuffers(1, &mRendererID));
	}

	void VertexBuffer::AddData(const void* data, unsigned int size) const
	{
		Bind();
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}

	void VertexBuffer::AddStreamingData(unsigned int size) const
	{
		Bind();
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STREAM_DRAW));
	}

	void VertexBuffer::AddSubData(const void* data, unsigned int parentsize, unsigned int datasize) const
	{
		Bind();
		GLCall(glBufferData(GL_ARRAY_BUFFER, parentsize, NULL, GL_STREAM_DRAW));
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, datasize, data));
	}

	void VertexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRendererID));
	}

	void VertexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}
