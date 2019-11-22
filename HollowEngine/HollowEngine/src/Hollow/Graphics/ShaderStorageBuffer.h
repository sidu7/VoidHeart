#pragma once
#include "Hollow/Common.h"

namespace Hollow
{
	class ShaderStorageBuffer
	{
	private:
		unsigned int mSize;
		unsigned int mRendererID;
	public:
		HOLLOW_API ShaderStorageBuffer();
		HOLLOW_API ~ShaderStorageBuffer();
		 
		HOLLOW_API void Bind(unsigned int bindPoint) const;
		HOLLOW_API void Unbind(unsigned int bindPoint) const;
		HOLLOW_API void CreateBuffer(unsigned int size);
		HOLLOW_API void* GetBufferWritePointer(const bool invalidateOldData = false) const;
		HOLLOW_API void* GetBufferReadPointer() const;
		HOLLOW_API void* GetBufferReadWritePointer() const;
		HOLLOW_API void ReleaseBufferPointer() const;
		HOLLOW_API static void PutMemoryBarrier();
	};
}