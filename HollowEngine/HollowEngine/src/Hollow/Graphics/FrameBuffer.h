#pragma once

namespace Hollow {

	class FrameBuffer
	{
	private:
		unsigned int mRendererID;
		unsigned int mPrevWidth, mPrevHeight;

	public:
		unsigned int *mpTextureID;
		unsigned int mWidth, mHeight, mDepthBuffer;
		unsigned int mTexCount;
	public:
		FrameBuffer(int width, int height, int Texcount = 1, bool attachDepth = false);
		FrameBuffer() { }
		~FrameBuffer();

		void Bind();
		void Unbind() const;
		void TexBind(unsigned int index = 0, unsigned int slot = 0);
		void TexUnbind(unsigned int index = 0);
		void Clear() const;
		void Delete() const;
		void ChangeSize(unsigned int width, unsigned int height);
	};
}