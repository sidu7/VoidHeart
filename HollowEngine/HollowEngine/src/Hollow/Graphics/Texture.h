#pragma once

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
		HOLLOW_API Texture(const std::string& FilePath);
		HOLLOW_API Texture(void* buffer, int size);
		HOLLOW_API Texture(int channels, int width, int height);
		HOLLOW_API Texture() {}
		HOLLOW_API ~Texture();
		 
		HOLLOW_API void Bind(unsigned int slot = 0) const;
		HOLLOW_API void Unbind(unsigned int slot = 0) const;
		HOLLOW_API void EnableTiling() const;
		HOLLOW_API unsigned int GetWidth() const { return mWidth; }
		HOLLOW_API unsigned int GetHeight() const { return mHeight; }
		HOLLOW_API unsigned int GetChannels() const { return mChannels; }
		HOLLOW_API unsigned int GetTextureID() const { return mRendererID; }

	private:
		void ReadBufferToTexture();
		void ReadBufferToHDRTexture(float* data);
	};
}