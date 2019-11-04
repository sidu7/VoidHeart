#pragma once

namespace Hollow
{
	class HOLLOW_API Texture
	{
	private:
		unsigned int mRendererID;
		unsigned char* mLocalBuffer;

	public:
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
		inline unsigned int GetTextureID() { return mRendererID; }

	private:
		void ReadBufferToTexture();
	};
}