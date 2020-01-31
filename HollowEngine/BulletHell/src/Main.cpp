#include <Hollow.h>
#include "Hollow/Common.h"
#include "Hollow/Core/EntryPoint.h"

#include "Hollow/Managers/GameObjectManager.h"
#include "Hollow/Managers/MemoryManager.h"
#include "Hollow/Managers/SceneManager.h"
#include "Hollow/Managers/RenderManager.h"
#include "Hollow/Managers/ResourceManager.h"
#include "Hollow/Managers/EventManager.h"

#include "Components/Health.h"
#include "Components/ParentOffset.h"

#include "GameMetaData/GameObjectType.h"
#include "GameMetaData/GameEventType.h"
#include "Hollow/Core/GameMetaData.h"

#include "DungeonGeneration/DungeonManager.h"

#include "Hollow/Components/Body.h"
#include "Hollow/Components/UITransform.h"

void Hollow::GameMetaData::Init()
{
	{
#define GAMEOBJECT_TYPE(name) mMapOfGameObjectTypes[#name] = (int)BulletHell::GameObjectType::name;
#include "GameMetaData/GameObjectType.enum"
#undef GAMEOBJECT_TYPE
	}

	{
#define GAME_EVENT(name) mMapOfGameEventTypes[#name] = (int)BulletHell::GameEventType::name;
#include "GameMetaData/GameEvents.enum"
#undef GAME_EVENT
	}
}

class GameLayer : public Hollow::Layer
{
	void OnUpdate()
	{
		// Update Game managers here
	}
};

class BulletHellGame : public Hollow::Application
{
public:
	BulletHellGame()
	{
		Hollow::GameMetaData::Instance().Init();
		// Engine Initialization
		Application::Init("Resources/Settings.json");

		Hollow::SceneManager::Instance().LoadLevel("Level3");

		PushLayer(new GameLayer());

        BulletHell::DungeonManager::Instance().Init();
		//BulletHell::DungeonManager::Instance().Generate();
		// Create temporary spell object
		Hollow::ResourceManager::Instance().LoadPrefabAtPosition("FireballSpell", glm::vec3(125.0, 1.0, 65.0));
		Hollow::GameObject* pLeftHand = Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Prefabs/Hand.json");
		
		BulletHell::ParentOffset* pParentOffset = pLeftHand->GetComponent<BulletHell::ParentOffset>();
		pParentOffset->mOffset = glm::vec3(1.0f, 0.0f, 0.0f);

		Hollow::UITransform* pUITr = pLeftHand->GetComponent<Hollow::UITransform>();
		pUITr->mPosition.x = 1100.0f;

		Hollow::GameObject* pRightHand = Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Prefabs/Hand.json");

		pParentOffset = pRightHand->GetComponent<BulletHell::ParentOffset>();
		pParentOffset->mOffset = glm::vec3(-1.0f, 0.0f, 0.0f);

	}

	~BulletHellGame()
	{
		HW_TRACE("BulletHell Prototype Closing");
	}

};


Hollow::Application* Hollow::CreateApplication()
{
	return new BulletHellGame();
}