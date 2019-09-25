#pragma once

namespace Hollow {

	class ElementArrayBuffer
	{
	private:
		unsigned int mRendererID;
		unsigned int mCount;
	public:
		ElementArrayBuffer();
		~ElementArrayBuffer();

		void AddData(const void* data, unsigned int count, unsigned int size);
		void Bind() const;
		void Unbind() const;
		inline unsigned int GetCount() const { return mCount; }
	};
}