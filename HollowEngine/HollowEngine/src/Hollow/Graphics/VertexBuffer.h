/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/

#pragma once
#include "Hollow/Common.h"

namespace Hollow {
	class VertexBuffer
	{
	private:
		unsigned int mRendererID;
		unsigned int mCount;
		unsigned int mSize;
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="VertexBuffer"/> class.
		/// </summary>
		HOLLOW_API VertexBuffer();

		/// <summary>
		/// Destructor for an instance of the <see cref="VertexBuffer"/> class.
		/// </summary>
		HOLLOW_API ~VertexBuffer();
		 
		/// <summary>
		/// Loads data from CPU into buffer for use on GPU
		/// </summary>
		/// <param name="data">Pointer to the bytes of data</param>
		/// <param name="count">The number of data objects being sent</param>
		/// <param name="size">The size of one data object</param>
		HOLLOW_API void AddData(const void* data, size_t count, size_t size);

		/// <summary>
		/// Adds the streaming data.
		/// </summary>
		/// <param name="size">The size of data in bytes </param>
		HOLLOW_API void AddStreamingData(unsigned int size) const;

		/// <summary>
		/// Adds the sub data from GPU into buffer for use on GPU
		/// </summary>
		/// <param name="data">Pointer to the bytes of data</param>
		/// <param name="count">The number of data objects being sent</param>
		/// <param name="size">The size of one data object</param>
		HOLLOW_API void AddSubData(const void* data, unsigned int count, unsigned int size);

		/// <summary>
		/// Binds the vertex buffer to GL_ARRAY_BUFFER
		/// </summary>
		HOLLOW_API void Bind() const;

		/// <summary>
		/// Unbinds the vertex buffer from GL_ARRAY_BUFFER
		/// </summary>
		HOLLOW_API void Unbind() const;

		/// <summary>
		/// Gets the number vertices in the buffer
		/// </summary>
		/// <returns>Number of vertices</returns>
		HOLLOW_API unsigned int GetVerticesCount() { return mCount; }
		
		HOLLOW_API unsigned int GetSize() { return mSize; }

		HOLLOW_API void* GetBufferReadPointer() const;

		HOLLOW_API void ReleaseBufferPointer() const;
	};
}