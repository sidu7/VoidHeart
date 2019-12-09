#pragma once
#include "Hollow/Common.h"

namespace Hollow {

	class ElementArrayBuffer
	{
	private:
		unsigned int mRendererID;
		unsigned int mCount;
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="ElementArrayBuffer"/> class.
		/// </summary>
		HOLLOW_API ElementArrayBuffer();
		/// <summary>
		/// Destructor for an instance of the <see cref="ElementArrayBuffer"/> class.
		/// </summary>
		HOLLOW_API ~ElementArrayBuffer();
		 
		/// <summary>
		/// Adds data to the element array buffer
		/// </summary>
		/// <param name="data">A pointer to the data to be stored</param>
		/// <param name="count">The number of data elements to be added</param>
		/// <param name="size">The size of one data element in bytes</param>
		HOLLOW_API void AddData(const void* data, size_t count, size_t size);

		/// <summary>
		/// Binds the buffer to GL_ELEMENT_ARRAY_BUFFER
		/// </summary>
		HOLLOW_API void Bind() const;

		/// <summary>
		/// Unbinds GL_ELEMENT_ARRAY_BUFFER
		/// </summary>
		HOLLOW_API void Unbind() const;

		/// <summary>
		/// Gets the number of elements in the buffer
		/// </summary>
		/// <returns>Number of elements in the buffer</returns>
		HOLLOW_API inline unsigned int GetCount() const { return mCount; }
	};
}