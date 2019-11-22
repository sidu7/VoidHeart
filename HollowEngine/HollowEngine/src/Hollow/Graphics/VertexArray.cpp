/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/

#include <hollowpch.h>

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
			if (c.mType == GL_INT)
			{
				GLCall(glVertexAttribIPointer(mIndex, c.mCount, c.mType, mStride, (const void*)(intptr_t)mOffset));
			}
			else
			{
				GLCall(glVertexAttribPointer(mIndex, c.mCount, c.mType, GL_FALSE, mStride, (const void*)(intptr_t)mOffset));
			}
			mOffset += c.mSize * c.mCount;
			mIndex++;
		}
		mElements.clear();
		mStride = 0;
		mOffset = 0;
	}

	void VertexArray::Push(unsigned int count, unsigned int type, unsigned int size)
	{
		VertexElements element(count, type, size);
		mElements.push_back(element);
		mStride += count * size;
	}

	void VertexArray::PushMatrix(unsigned int count, unsigned int type, unsigned int size, unsigned int vecsize)
	{
		GLCall(glEnableVertexAttribArray(mIndex));
		GLCall(glVertexAttribPointer(mIndex, count, type, GL_FALSE, size, (const void*)(intptr_t)0));
		GLCall(glVertexAttribDivisor(mIndex++, 1));
		GLCall(glEnableVertexAttribArray(mIndex));
		GLCall(glVertexAttribPointer(mIndex, count, type, GL_FALSE, size, (const void*)(intptr_t)(vecsize)));
		GLCall(glVertexAttribDivisor(mIndex++, 1));
		GLCall(glEnableVertexAttribArray(mIndex));
		GLCall(glVertexAttribPointer(mIndex, count, type, GL_FALSE, size, (const void*)(intptr_t)(2 * vecsize)));
		GLCall(glVertexAttribDivisor(mIndex++, 1));
		GLCall(glEnableVertexAttribArray(mIndex));
		GLCall(glVertexAttribPointer(mIndex, count, type, GL_FALSE, size, (const void*)(intptr_t)(3 * vecsize)));
		GLCall(glVertexAttribDivisor(mIndex++, 1));
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