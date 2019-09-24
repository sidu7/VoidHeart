#include <hollowpch.h>
#include "ResourceManager.h"
#include "Hollow/Core/GameObjectFactory.h"
#include "GameObjectManager.h"

void Hollow::ResourceManager::LoadGameObjectFromFile(std::string path)
{
	std::ifstream file(path);
	std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	rapidjson::Document root;
	root.Parse(contents.c_str());
	
	if (root.IsObject()) {
		GameObject* pNewGameObject = GameObjectFactory::Instance().LoadObject(root.GetObject());

		if (pNewGameObject)
		{
			GameObjectManager::Instance().AddGameObject(pNewGameObject);
		}
	}
}

Hollow::Texture* Hollow::ResourceManager::LoadTexture(std::string path)
{
	//Check in cache
	if (mTextureCache.find(path) != mTextureCache.end())
	{
		return mTextureCache[path];
	}
	
	//Texture* texture = new Texture(path);
	//mTextureCache[path] = texture;
	//return texture;
}

Hollow::Model* Hollow::ResourceManager::LoadModel(std::string path)
{
	//Check in cache
	if (mModelCache.find(path) != mModelCache.end())
	{
		return mModelCache[path];
	}

	//Model* model = new Model(path);
	//mModelCache[path] = model;
	//return model;
}
