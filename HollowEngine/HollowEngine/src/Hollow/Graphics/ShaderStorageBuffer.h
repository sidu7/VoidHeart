#pragma once
#include "Hollow/Common.h"

namespace Hollow
{
	class ShaderStorageBuffer
	{
	public:
		unsigned int mSize;
		unsigned int mRendererID;
		/// <summary>
		/// Initializes a new instance of the <see cref="ShaderStorageBuffer"/> class.
		/// </summary>
		HOLLOW_API ShaderStorageBuffer();

		/// <summary>
		/// Destructor for an instance of the <see cref="ShaderStorageBuffer"/> class.
		/// </summary>
		HOLLOW_API ~ShaderStorageBuffer();
		 
		/// <summary>
		/// Binds the buffer object to an indexed buffer target
		/// </summary>
		/// <param name="bindPoint">The index of the binding point</param>
		HOLLOW_API void Bind(unsigned int bindPoint) const;

		/// <summary>
		/// Unbinds the buffer object at an indexed buffer target
		/// </summary>
		/// <param name="bindPoint">The index of the binding point</param>
		HOLLOW_API void Unbind(unsigned int bindPoint) const;

		/// <summary>
		/// Creates the buffer and buffer storage
		/// </summary>
		/// <param name="size">The size in bytes of buffer objects data store</param>
		HOLLOW_API void CreateBuffer(unsigned int size);

		/// <summary>
		/// Gets the buffer write pointer
		/// </summary>
		/// <param name="invalidateOldData">Flag to set if old data should be invalidated</param>
		/// <returns>Buffer write pointer</returns>
		HOLLOW_API void* GetBufferWritePointer(const bool invalidateOldData = false) const;

		/// <summary>
		/// Gets the buffer read pointer
		/// </summary>
		/// <returns>Buffer read pointer</returns>
		HOLLOW_API void* GetBufferReadPointer() const;

		/// <summary>
		/// Gets the buffer read write pointer
		/// </summary>
		/// <returns>Buffer read write pointer</returns>
		HOLLOW_API void* GetBufferReadWritePointer() const;

		/// <summary>
		/// Release the mapping of a buffer object's data store into the client's address space
		/// </summary>
		HOLLOW_API void ReleaseBufferPointer() const;

		/// <summary>
		/// Defines a barrier ordering memory transactions
		/// Accesses to shader storage blocks after the barrier will reflect writes prior to the barrier
		/// </summary>
		HOLLOW_API static void PutMemoryBarrier();
	};
}