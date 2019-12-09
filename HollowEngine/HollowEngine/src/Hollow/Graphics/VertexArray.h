/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/

#pragma once
#include <list>
#include "Hollow/Common.h"

namespace Hollow {

	class VertexBuffer;

	class VertexElements
	{
	public:
		unsigned int mCount;
		unsigned int mType;
		unsigned int mSize;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="VertexElements"/> class.
		/// </summary>
		/// <param name="count">The number of elements in the layout</param>
		/// <param name="type">Unsigned int used to specify data type</param>
		/// <param name="size">The size of the type in bytes</param>
		HOLLOW_API VertexElements(unsigned int count, unsigned int type, unsigned int size);

	};

	class VertexArray
	{
	private:
		unsigned int mRendererID;
		unsigned int mStride;
		unsigned int mOffset;
		unsigned int mIndex;
		std::list<VertexElements> mElements;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="VertexArray"/> class.
		/// </summary>
		HOLLOW_API VertexArray();

		/// <summary>
		/// Destructor for an instance of the <see cref="VertexArray"/> class.
		/// </summary>
		HOLLOW_API ~VertexArray();
		 
		/// <summary>
		/// Adds all currently pushed vertex elements to the VAO layout
		/// </summary>
		HOLLOW_API void AddLayout();

		/// <summary>
		/// Binds the specified vertex buffer
		/// </summary>
		/// <param name="vb">The vertex buffer to bind</param>
		HOLLOW_API void AddBuffer(const VertexBuffer& vb) const;

		/// <summary>
		/// Creates and adds a vertex element to the VAO
		/// </summary>
		/// <param name="count">The number of elements in the layout</param>
		/// <param name="type">Unsigned int used to specify data type</param>
		/// <param name="size">The size of the type in bytes</param>
		HOLLOW_API void Push(unsigned int count, unsigned int type, unsigned int size);

		/// <summary>
		/// Pushes a matrix onto the VAO
		/// </summary>
		/// <param name="count">The number of elements in the layout</param>
		/// <param name="type">Unsigned int used to specify data type</param>
		/// <param name="size">The size of the type in bytes</param>
		/// <param name="vecsize">The desired vector size</param>
		HOLLOW_API void PushMatrix(unsigned int count, unsigned int type, unsigned int size, unsigned int vecsize);

		/// <summary>
		/// Binds this VAO
		/// </summary>
		HOLLOW_API void Bind() const;

		/// <summary>
		/// Unbinds this VAO
		/// </summary>
		HOLLOW_API void Unbind() const;

		/// <summary>
		/// Deletes the VAO generated for use on the GPU
		/// </summary>
		HOLLOW_API void Delete() const;
	};
}