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
		~Texture();

		void Bind(unsigned int slot = 0) const;
		void Unbind() const;
		void EnableTiling() const;
	};
}