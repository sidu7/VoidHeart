#pragma once

namespace Hollow {

	class ElementArrayBuffer
	{
	private:
		unsigned int mRendererID;
		unsigned int mCount;
	public:
		HOLLOW_API ElementArrayBuffer();
		HOLLOW_API ~ElementArrayBuffer();
		 
		HOLLOW_API void AddData(const void* data, unsigned int count, unsigned int size);
		HOLLOW_API void Bind() const;
		HOLLOW_API void Unbind() const;
		HOLLOW_API inline unsigned int GetCount() const { return mCount; }
	};
}