#include <Hollow.h>
#include "Hollow/Common.h"
#include "Hollow/Core/EntryPoint.h"

#include "Hollow/Managers/GameObjectManager.h"
#include "Hollow/Managers/MemoryManager.h"
#include "Hollow/Managers/RenderManager.h"
#include "Hollow/Managers/ResourceManager.h"
#include "Hollow/Managers/EventManager.h"
#include "Hollow/Managers/SceneManager.h"

#include "Hollow/Core/GameMetaData.h"
#include "GameMetaData/GameObjectType.h"
#include "GameMetaData/GameEventType.h"

void Hollow::GameMetaData::Init()
{
	{
#define GAMEOBJECT_TYPE(name) mMapOfGameObjectTypes[#name] = (int)MarbleBall::GameObjectType::name;
#include "GameMetaData/GameObjectType.enum"
#undef GAMEOBJECT_TYPE
	}

	{
#define GAME_EVENT(name) mMapOfGameEventTypes[#name] = (int)MarbleBall::GameEventType::name;
#include "GameMetaData/GameEvents.enum"
#undef GAME_EVENT
	}
}

namespace MarbleBall
{
	class MarbleBall : public Hollow::Application
	{
	public:
		MarbleBall()
		{
			Hollow::GameMetaData::Instance().Init();
			// Engine Initialization
			Application::Init("Resources/Settings.json");

			Hollow::SceneManager::Instance().LoadLevel("Marble");
			Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/UICamera.json");
		}

		~MarbleBall()
		{
			HW_TRACE("MarbleBall Closing");
		}
	};
}

Hollow::Application* Hollow::CreateApplication()
{
	return new MarbleBall::MarbleBall();
}