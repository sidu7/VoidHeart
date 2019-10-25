#pragma once

namespace Hollow
{
	class HOLLOW_API ShaderStorageBuffer
	{
	private:
		unsigned int mSize;
	public:
		unsigned int mRendererID;
		ShaderStorageBuffer();
		~ShaderStorageBuffer();

		void Bind(unsigned int bindPoint) const;
		void Unbind() const;
		void CreateBuffer(unsigned int size);
		void* GetBufferWritePointer() const;
		void ReleaseBufferPointer() const;
	};
}