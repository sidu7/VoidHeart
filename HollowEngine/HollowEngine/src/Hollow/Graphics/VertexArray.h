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

	class HOLLOW_API VertexElements
	{
	public:
		unsigned int mCount;
		unsigned int mType;
		unsigned int mSize;

	public:
		VertexElements(unsigned int count, unsigned int type, unsigned int size);

	};

	class HOLLOW_API VertexArray
	{
	private:
		unsigned int mRendererID;
		unsigned int mStride;
		unsigned int mOffset;
		unsigned int mIndex;
		std::list<VertexElements> mElements;

	public:
		VertexArray();
		~VertexArray();

		void AddLayout();
		void AddBuffer(const VertexBuffer& vb) const;
		void Push(unsigned int count, unsigned int type, unsigned int size);
		void PushMatrix(unsigned int count, unsigned int type, unsigned int size, unsigned int vecsize);
		void Bind() const;
		void Unbind() const;
		void Delete() const;
	};
}