#pragma once

namespace Hollow {

	class FrameBuffer
	{
	private:
		unsigned int mRendererID;
		glm::ivec2 mScreenViewPort;
		glm::ivec2 mScreenPosition;

	public:
		unsigned int *mpTextureID;
		unsigned int mWidth, mHeight, mDepthBuffer;
		unsigned int mTexCount;
	public:
		HOLLOW_API FrameBuffer(int width, int height, int Texcount = 1, bool attachDepth = false);
		HOLLOW_API FrameBuffer() { }
		HOLLOW_API ~FrameBuffer();

		HOLLOW_API void Bind();
		HOLLOW_API void BindRead();
		HOLLOW_API void Unbind() const;
		HOLLOW_API void TexBind(unsigned int index = 0, unsigned int slot = 0);
		HOLLOW_API void TexUnbind(unsigned int slot = 0);
		HOLLOW_API void Clear() const;
		HOLLOW_API void Delete() const;
		HOLLOW_API void ChangeSize(unsigned int width, unsigned int height);
		HOLLOW_API unsigned int GetFrameBufferID() const { return mRendererID; }
	};
}