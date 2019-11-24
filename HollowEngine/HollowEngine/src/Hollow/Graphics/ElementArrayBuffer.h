#pragma once
#include "Hollow/Common.h"

namespace Hollow {

	class ElementArrayBuffer
	{
	private:
		unsigned int mRendererID;
		unsigned int mCount;
	public:
		HOLLOW_API ElementArrayBuffer();
		HOLLOW_API ~ElementArrayBuffer();
		 
		HOLLOW_API void AddData(const void* data, size_t count, size_t size);
		HOLLOW_API void Bind() const;
		HOLLOW_API void Unbind() const;
		HOLLOW_API inline unsigned int GetCount() const { return mCount; }
	};
}