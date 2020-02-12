#include <hollowpch.h>
#include "Hollow/Core/GameObjectFactory.h"
#include "Hollow/Core/GameObject.h"
#include "Hollow/Components/Component.h"
#include "Hollow/Managers/MemoryManager.h"
#include "GameMetaData.h"

namespace Hollow {
		
	GameObject* GameObjectFactory::LoadObject(rapidjson::Value::Object root)
	{
		GameObject* pNewGameObject = MemoryManager::Instance().NewGameObject();
		
		//unsigned int objectID = root["ID"].GetUint();
		//pNewGameObject->mID = objectID;

		if (root.HasMember("Type"))
		{
			pNewGameObject->mDType = root["Type"].GetString();
			pNewGameObject->mType = GameMetaData::Instance().mMapOfGameObjectTypes[pNewGameObject->mDType];
		}

		if(root.HasMember("Active"))
		{
			pNewGameObject->mActive = root["Active"].GetBool();
		}

		if (root.HasMember("Tag"))
		{
			pNewGameObject->mTag = root["Tag"].GetString();
		}
		
		if (root.HasMember("Components"))
		{
			rapidjson::Value::Array comp = root["Components"].GetArray();
			for (unsigned int i = 0; i < comp.Size(); ++i)
			{
				std::string Comptype = comp[i]["Type"].GetString();

				Component* pComp = MemoryManager::Instance().NewComponent(Comptype);
				pNewGameObject->AddComponent(pComp);
				pComp->Serialize(comp[i].GetObject());
			}
		}

		return pNewGameObject;
	}
}
