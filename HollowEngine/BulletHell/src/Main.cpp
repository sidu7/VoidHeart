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
#include "GameMetaData/GameObjectType.h"
#include "GameMetaData/GameEventType.h"
#include "Hollow/Core/GameMetaData.h"

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

class BulletHellGame : public Hollow::Application
{
public:
	BulletHellGame()
	{
		Hollow::GameMetaData::Instance().Init();
		// Engine Initialization
		Application::Init("Resources/Settings.json");


		Hollow::SceneManager::Instance().LoadLevel("Level1");
		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Floor.json");
		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Camera.json");
		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/TempObject.json");
		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Player.json");
		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Cloud.json");
		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/NPC.json");
		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Enemy.json");
		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Plane.json");

		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/UICamera.json");
		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/MiniMapCamera.json");
		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Light.json");
		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Quad.json");


		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Wall.json");

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