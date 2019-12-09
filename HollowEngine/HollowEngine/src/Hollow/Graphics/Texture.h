#pragma once
#include "Hollow/Common.h"

namespace Hollow
{
	class Texture
	{
	private:
		unsigned int mRendererID;
		unsigned char* mLocalBuffer;
		int mWidth, mHeight, mChannels;

	public:
		std::string mFilePath;
		/// <summary>
		/// Initializes a new instance of the <see cref="Texture"/> class
		/// </summary>
		/// <param name="FilePath">The file path to the texture location</param>
		HOLLOW_API Texture(const std::string& FilePath);

		/// <summary>
		/// Initializes a new instance of the <see cref="Texture"/> class
		/// </summary>
		/// <param name="buffer">The buffer to load texture from</param>
		/// <param name="size">The size of the memory in bytes</param>
		HOLLOW_API Texture(void* buffer, int size);

		/// <summary>
		/// Initializes a new instance of the <see cref="Texture"/> class
		/// </summary>
		/// <param name="channels">The number of color channels for the texture</param>
		/// <param name="width">The width of the texture in pixels</param>
		/// <param name="height">The height of the texture in pixels</param>
		HOLLOW_API Texture(int channels, int width, int height);

		/// <summary>
		/// Initializes a new instance of the <see cref="Texture"/> class
		/// </summary>
		HOLLOW_API Texture() {}

		/// <summary>
		/// Destructor for an instance of the <see cref="Texture"/> class
		/// </summary>
		HOLLOW_API ~Texture();
		 
		/// <summary>
		/// Binds the texture to the specified slot
		/// </summary>
		/// <param name="slot">The texture unit slot to use</param>
		HOLLOW_API void Bind(unsigned int slot = 0) const;

		/// <summary>
		/// Unbinds the texture in the specified slot
		/// </summary>
		/// <param name="slot">The texture unit slot to unbind</param>
		HOLLOW_API void Unbind(unsigned int slot = 0) const;

		/// <summary>
		/// Enables texture tiling
		/// </summary>
		HOLLOW_API void EnableTiling() const;

		/// <summary>
		/// Gets the width of the texture in pixels
		/// </summary>
		/// <returns>Width of the texture in pixels</returns>
		HOLLOW_API unsigned int GetWidth() const { return mWidth; }

		/// <summary>
		/// Gets the height of the texture in pixels
		/// </summary>
		/// <returns>Height of the texture in pixels</returns>
		HOLLOW_API unsigned int GetHeight() const { return mHeight; }

		/// <summary>
		/// Gets number of color channels in the texture
		/// </summary>
		/// <returns>Number of color channels</returns>
		HOLLOW_API unsigned int GetChannels() const { return mChannels; }

		/// <summary>
		/// Gets the texture renderer ID used by OpenGL
		/// </summary>
		/// <returns>Texture renderer ID</returns>
		HOLLOW_API unsigned int GetTextureID() const { return mRendererID; }

	private:
		void ReadBufferToTexture();
		void ReadBufferToHDRTexture(float* data);
	};
}