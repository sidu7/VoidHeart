#pragma once

namespace Hollow
{
	class HOLLOW_API Texture
	{
	private:
		unsigned int mRendererID;
		unsigned char* mLocalBuffer;
		int mWidth, mHeight, mChannels;

	public:
		std::string mFilePath;
		Texture(const std::string& FilePath);
		Texture(void* buffer, int size);
		Texture(int channels, int width, int height);
		Texture() {}
		~Texture();

		void Bind(unsigned int slot = 0) const;
		void Unbind(unsigned int slot = 0) const;
		void EnableTiling() const;
		unsigned int GetWidth() const { return mWidth; }
		unsigned int GetHeight() const { return mHeight; }
		unsigned int GetChannels() const { return mChannels; }
		unsigned int GetTextureID() const { return mRendererID; }

	private:
		void ReadBufferToTexture();
		void ReadBufferToHDRTexture(float* data);
	};
}