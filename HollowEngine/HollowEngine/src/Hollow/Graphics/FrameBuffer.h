#pragma once
#include "Hollow/Common.h"

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
		/// <summary>
		/// Initializes a new instance of the <see cref="FrameBuffer"/> class.
		/// </summary>
		/// <param name="width">The desired width of the framebuffer</param>
		/// <param name="height">The desired height of the framebuffer</param>
		/// <param name="Texcount">The number of texture to bind to the framebuffer</param>
		/// <param name="attachDepth">Flag to determine if a depth buffer should be attached</param>
		HOLLOW_API FrameBuffer(int width, int height, int Texcount = 1, bool attachDepth = false);

		/// <summary>
		/// Initializes a new instance of the <see cref="FrameBuffer"/> class.
		/// </summary>
		HOLLOW_API FrameBuffer() { }

		/// <summary>
		/// Destructor for an instance of the <see cref="FrameBuffer"/> class.
		/// </summary>
		HOLLOW_API ~FrameBuffer();

		/// <summary>
		/// Bind the framebuffer to GL_FRAMEBUFFER
		/// </summary>
		HOLLOW_API void Bind();

		/// <summary>
		/// Bind the framebuffer to GL_READ_FRAMEBUFFER
		/// </summary>
		HOLLOW_API void BindRead();

		/// <summary>
		/// Unbinds the framebuffer and reset viewport
		/// </summary>
		HOLLOW_API void Unbind() const;

		/// <summary>
		/// Binds a specified texture from the framebuffer
		/// </summary>
		/// <param name="index">The index framebuffer texture to use</param>
		/// <param name="slot">The active texture slot to use</param>
		HOLLOW_API void TexBind(unsigned int index = 0, unsigned int slot = 0);

		/// <summary>
		/// Unbinds a specified texture from the framebuffer
		/// </summary>
		/// <param name="slot">The active texture slot to use</param>
		HOLLOW_API void TexUnbind(unsigned int slot = 0);

		/// <summary>
		/// Clears the color and depth buffer bits
		/// </summary>
		HOLLOW_API void Clear() const;

		/// <summary>
		/// Deletes the framebuffer
		/// </summary>
		HOLLOW_API void Delete() const;

		/// <summary>
		/// Changes the size of the framebuffer
		/// </summary>
		/// <param name="width">The new width to use</param>
		/// <param name="height">The new height to use</param>
		HOLLOW_API void ChangeSize(unsigned int width, unsigned int height);

		/// <summary>
		/// Gets the frame buffer renderer ID
		/// </summary>
		/// <returns>Framebuffer rendererID</returns>
		HOLLOW_API unsigned int GetFrameBufferID() const { return mRendererID; }
	};
}