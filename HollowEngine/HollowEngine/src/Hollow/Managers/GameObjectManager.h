#pragma once
#include "hollowpch.h"
#include"..//GameObject.h"

namespace Hollow{

	class GameObjectManager 
	{
	public:
		void AddGameObject(GameObject* object);
		void DeleteGameObject(GameObject* object);
		void DeleteAllGameObjects();

	private:
		std::vector<GameObject*> mGameObjects;
	};
}