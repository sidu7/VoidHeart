#pragma once

namespace Hollow
{
	class UniformBuffer
	{
	private:
		unsigned int m_RendererId;
	public:
		HOLLOW_API UniformBuffer(unsigned int size, const void* data = nullptr);
		HOLLOW_API ~UniformBuffer();
		
		HOLLOW_API void Bind(unsigned int bindPoint) const;
		HOLLOW_API void Unbind() const;
		HOLLOW_API void SubData(unsigned int size, const void* data) const;
	};
}