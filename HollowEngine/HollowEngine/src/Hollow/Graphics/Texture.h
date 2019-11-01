#pragma once

namespace Hollow
{
	class HOLLOW_API Texture
	{
	private:
		unsigned int mRendererID;
		int mWidth, mHeight, mBPP;
		unsigned char* mLocalBuffer;

	public:
		std::string mFilePath;
		Texture(const std::string& FilePath);
		Texture(void* buffer, int size);
		~Texture();

		void Bind(unsigned int slot = 0) const;
		void Unbind(unsigned int slot = 0) const;
		void EnableTiling() const;
		inline unsigned int GetRendererID() { return mRendererID; }

	private:
		void ReadBufferToTexture();
	};
}