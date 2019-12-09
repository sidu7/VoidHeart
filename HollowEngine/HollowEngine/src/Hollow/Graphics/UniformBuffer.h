#pragma once

namespace Hollow
{
	class UniformBuffer
	{
	private:
		unsigned int m_RendererId;
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="UniformBuffer"/> class.
		/// </summary>
		/// <param name="size">The size of the data in bytes</param>
		/// <param name="data">Pointed to that data to bind</param>
		HOLLOW_API UniformBuffer(unsigned int size, const void* data = nullptr);

		/// <summary>
		/// Destructor for an instance of the <see cref="UniformBuffer"/> class.
		/// </summary>
		HOLLOW_API ~UniformBuffer();
		
		/// <summary>
		/// Binds buffer object to an indexed buffer target
		/// </summary>
		/// <param name="bindPoint">Index of the binding point</param>
		HOLLOW_API void Bind(unsigned int bindPoint) const;

		/// <summary>
		/// Unbinds GL_UNIFORM_BUFFER
		/// </summary>
		HOLLOW_API void Unbind() const;

		/// <summary>
		/// Sets subdata in GL_UNIFORM_BUFFER
		/// </summary>
		/// <param name="size">The size of the data in bytes</param>
		/// <param name="data">Pointed to that data to bind</param>
		HOLLOW_API void SubData(unsigned int size, const void* data) const;
	};
}