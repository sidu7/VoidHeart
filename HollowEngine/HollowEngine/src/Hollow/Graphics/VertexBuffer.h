/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/

#pragma once

namespace Hollow {
	class VertexBuffer
	{
	private:
		unsigned int mRendererID;
		unsigned int mCount;
	public:
		HOLLOW_API VertexBuffer();
		HOLLOW_API ~VertexBuffer();
		 
		HOLLOW_API void AddData(const void* data, size_t count, size_t size);
		HOLLOW_API void AddStreamingData(unsigned int size) const;
		HOLLOW_API void AddSubData(const void* data, unsigned int count, unsigned int size);
		HOLLOW_API void Bind() const;
		HOLLOW_API void Unbind() const;
		HOLLOW_API unsigned int GetVerticesCount() { return mCount; }
	};
}