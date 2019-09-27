#include <hollowpch.h>
#include "Utils/GLCall.h"
#include "ElementArrayBuffer.h"

namespace Hollow {

	ElementArrayBuffer::ElementArrayBuffer()
	{
		GLCall(glGenBuffers(1, &mRendererID));
	}

	ElementArrayBuffer::~ElementArrayBuffer()
	{
		GLCall(glDeleteBuffers(1, &mRendererID));
	}

	void ElementArrayBuffer::AddData(const void* data, unsigned int count, unsigned int size)
	{
		mCount = count;
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * count, data, GL_STATIC_DRAW);
	}

	void ElementArrayBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID));
	}

	void ElementArrayBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
}
