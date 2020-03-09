#include <hollowpch.h>
#include "Hollow/Common.h"
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

	void ElementArrayBuffer::AddData(const void* data, size_t count, size_t size)
	{
		mCount = static_cast<unsigned>(count);
		mSize = static_cast<unsigned>(size * count);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mSize, data, GL_STATIC_DRAW);
	}

	void ElementArrayBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID));
	}

	void ElementArrayBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	void* ElementArrayBuffer::GetBufferReadPointer() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID));
		GLbitfield flag = GL_MAP_READ_BIT;
		return glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, mSize, flag);
	}

	void ElementArrayBuffer::ReleaseBufferPointer() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID));
		GLCall(glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER));
	}
}
