/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/

#pragma once

namespace Hollow {
	class HOLLOW_API VertexBuffer
	{
	private:
		unsigned int mRendererID;
		unsigned int mCount;
	public:
		VertexBuffer();
		~VertexBuffer();

		void AddData(const void* data, unsigned int count, unsigned int size);
		void AddStreamingData(unsigned int size) const;
		void AddSubData(const void* data, unsigned int count, unsigned int size);
		void Bind() const;
		void Unbind() const;
		inline unsigned int GetVerticesCount() { return mCount; }
	};
}