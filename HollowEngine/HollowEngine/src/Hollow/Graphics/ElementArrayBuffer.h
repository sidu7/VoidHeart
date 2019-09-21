#pragma once

namespace Hollow {

	class ElementArrayBuffer
	{
	private:
		unsigned int mRendererID;
	public:
		ElementArrayBuffer();
		~ElementArrayBuffer();

		void AddData(const void* data, unsigned int size) const;
		void Bind() const;
		void Unbind() const;
	};
}