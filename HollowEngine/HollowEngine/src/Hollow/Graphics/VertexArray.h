/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/

#pragma once
#include <list>

namespace Hollow {

	class VertexBuffer;

	class VertexElements
	{
	public:
		unsigned int mCount;
		unsigned int mType;
		unsigned int mSize;

	public:
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
		HOLLOW_API VertexArray();
		HOLLOW_API ~VertexArray();
		 
		HOLLOW_API void AddLayout();
		HOLLOW_API void AddBuffer(const VertexBuffer& vb) const;
		HOLLOW_API void Push(unsigned int count, unsigned int type, unsigned int size);
		HOLLOW_API void PushMatrix(unsigned int count, unsigned int type, unsigned int size, unsigned int vecsize);
		HOLLOW_API void Bind() const;
		HOLLOW_API void Unbind() const;
		HOLLOW_API void Delete() const;
	};
}