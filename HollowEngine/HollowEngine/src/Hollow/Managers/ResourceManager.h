#pragma once

namespace Hollow
{
	class Texture;
	class Model;

	class HOLLOW_API ResourceManager
	{
		SINGLETON(ResourceManager)
	public:
		~ResourceManager() {}

		void LoadGameObjectFromFile(std::string path);
		Texture* LoadTexture(std::string path);
		Model* LoadModel(std::string path);

	private:
		//Texture and Model cache
		std::unordered_map<std::string, Texture*> mTextureCache;
		std::unordered_map<std::string, Model*> mModelCache;
	};
}