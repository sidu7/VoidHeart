/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/

#include <hollowpch.h>
#include "Utils/GLCall.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace Hollow {

	VertexArray::VertexArray()
	{
		mStride = 0;
		mOffset = 0;
		mIndex  = 0;
		GLCall(glGenVertexArrays(1, &mRendererID));
	}

	VertexArray::~VertexArray()
	{
	}

	void VertexArray::AddLayout()
	{
		for (auto c : mElements)
		{
			GLCall(glEnableVertexAttribArray(mIndex));
			GLCall(glVertexAttribPointer(mIndex, c.mCount, c.mType, GL_FALSE, mStride, (const void*)mOffset));
			mOffset += c.mSize * c.mCount;
			mIndex++;
		}
	}

	void VertexArray::Push(unsigned int count, unsigned int type, unsigned int size)
	{
		VertexElements element(count, type, size);
		mElements.push_back(element);
		mStride += count * size;
	}

	void VertexArray::AddBuffer(const VertexBuffer& vb) const
	{
		Bind();
		vb.Bind();
	}

	void VertexArray::Bind() const
	{
		GLCall(glBindVertexArray(mRendererID));
	}

	void VertexArray::Unbind() const
	{
		GLCall(glBindVertexArray(0));
	}

	void VertexArray::Delete() const
	{
		GLCall(glDeleteVertexArrays(1, &mRendererID));
	}

	VertexElements::VertexElements(unsigned int count, unsigned int type, unsigned int size) :mCount(count), mType(type), mSize(size)
	{
	}
}