#include <Hollow.h>
#include "Hollow/Common.h"
#include "Hollow/Core/EntryPoint.h"

#include "Hollow/Managers/GameObjectManager.h"
#include "Hollow/Managers/MemoryManager.h"
#include "Hollow/Managers/SceneManager.h"
#include "Hollow/Managers/RenderManager.h"
#include "Hollow/Managers/ResourceManager.h"
#include "Hollow/Managers/EventManager.h"
#include "Hollow/Managers/ScriptingManager.h"

#include "Components/Health.h"
#include "Components/ParentOffset.h"

#include "GameMetaData/GameObjectType.h"
#include "GameMetaData/GameEventType.h"
#include "Hollow/Core/GameMetaData.h"

#include "DungeonGeneration/DungeonManager.h"
#include "Systems/HandSystem.h"

#include "Hollow/Components/Body.h"
#include "Hollow/Components/UITransform.h"
#include "GameLogic/GameLogicManager.h"
#include "Hollow/Managers/InputManager.h"

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
	void OnUpdate(float dt)
	{
		BulletHell::GameLogicManager::Instance().Update();
		
		if (BulletHell::GameLogicManager::hasGameStarted)
		{
			// Update Game managers here
			Hollow::ScriptingManager::Instance().RunScript("RoomLogic");
		}
	}

	void OnEvent(Hollow::Event& e)
	{
		if(e.GetEventType() == Hollow::EventType::WindowPaused)
		{
			BulletHell::GameLogicManager::Instance().TogglePause();
		}
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

		PushLayer(new GameLayer());
		Hollow::EventManager::Instance().SubscribeEvent((int)BulletHell::GameEventType::ON_EXIT_GAME, EVENT_CALLBACK(BulletHellGame::ExitGame));
		Hollow::EventManager::Instance().SubscribeEvent((int)BulletHell::GameEventType::TOGGLE_FULLSCREEN, EVENT_CALLBACK(BulletHellGame::ToggleFullScreenInGame));
		Hollow::EventManager::Instance().SubscribeEvent((int)BulletHell::GameEventType::TOGGLE_PAUSE, EVENT_CALLBACK(BulletHellGame::TogglePauseInGame));
		
		BulletHell::GameLogicManager::Instance().Init();
	}

	void TogglePauseInGame(Hollow::GameEvent& ge)
	{
		TogglePause();
	}
	
	void ToggleFullScreenInGame(Hollow::GameEvent& ge)
	{
		ToggleFullScreen();
	}
	
	void ExitGame(Hollow::GameEvent& ge)
	{
		ExitApplication();
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