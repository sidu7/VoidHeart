#include <hollowpch.h>
#include "Hollow/Core/GameObjectFactory.h"
#include "Hollow/Core/GameObject.h"
#include "Hollow/Components/TestComponent.h"
#include "Hollow/Managers/MemoryManager.h"

namespace Hollow {

	GameObject* GameObjectFactory::LoadObject(const char* pFileName)
	{
		GameObject* pNewGameObject = nullptr;

		std::ifstream file(pFileName);
		std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

		rapidjson::Document root;
		root.Parse(contents.c_str());

		if (root.IsObject()) {
			pNewGameObject = LoadObject(root.GetObject(), pFileName);
		}
		return pNewGameObject;
	}

	GameObject* GameObjectFactory::LoadObject(rapidjson::Value::Object root, std::string fname)
	{
		GameObject* pNewGameObject = nullptr;
		Component* pNewComponent = nullptr;

		std::string type = root["Type"].GetString();

		//New way to write components
		if (root.HasMember("Components"))
		{
			rapidjson::Value::Array comp = root["Components"].GetArray();
			for (unsigned int i = 0; i < comp.Size(); ++i)
			{
				std::string Comptype = comp[i]["Type"].GetString();

				Component* pComp = MemoryManager::Instance().NewComponent(Comptype.c_str());
				pNewGameObject->AddComponent(pComp);

				//[comp].CreateCompoent();

				//pNewComponent = pNewGameObject->AddComponent<typeid(map[""])>();
				//pNewComponent->Serialize(comp[i].GetObject());
				/*if (Comptype == "Particle")
				{
					ParticleSystem::GetInstance()->mParticleQueue.push_back((ParticleEmitter*)(pNewComponent));
				}*/
			}
		}

		return pNewGameObject;
	}
}