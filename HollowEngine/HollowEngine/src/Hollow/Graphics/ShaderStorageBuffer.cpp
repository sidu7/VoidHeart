#include <hollowpch.h>
#include "ShaderStorageBuffer.h"

#include "Utils/GLCall.h"

namespace Hollow
{
	ShaderStorageBuffer::ShaderStorageBuffer()
	{
		GLCall(glGenBuffers(1, &mRendererID));
	}

	ShaderStorageBuffer::~ShaderStorageBuffer()
	{
		GLCall(glDeleteBuffers(1, &mRendererID));
	}

	void ShaderStorageBuffer::Bind(unsigned int bindPoint) const
	{
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, mRendererID));
		GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindPoint, mRendererID));
	}

	void ShaderStorageBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
	}

	void ShaderStorageBuffer::CreateBuffer(unsigned int size)
	{
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, mRendererID));
		mSize = size;
		GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_STATIC_DRAW));
	}

	void ShaderStorageBuffer::ReleaseBufferPointer() const
	{
		GLCall(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
	}

	void ShaderStorageBuffer::PutMemoryBarrier() const
	{
		GLCall(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
	}

	void* ShaderStorageBuffer::GetBufferWritePointer() const
	{
		return glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, mSize, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	}

}