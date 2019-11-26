#include <Hollow.h>
#include "Hollow/Common.h"
#include "Hollow/Core/EntryPoint.h"

#include "Hollow/Managers/GameObjectManager.h"
#include "Hollow/Managers/MemoryManager.h"
#include "Hollow/Managers/RenderManager.h"
#include "Hollow/Managers/ResourceManager.h"
#include "Hollow/Managers/EventManager.h"
#include "Hollow/Managers/SceneManager.h"

#include "Hollow/Core/GameObjectMetaData.h"
#include "GameObjectsMetaData/GameObjectType.h"

void Hollow::GameObjectMetaData::Init()
{
	{
#define GAMEOBJECT_TYPE(name) mMapOfGameObjectTypes[#name] = (int)MarbleBall::GameObjectType::name;
#include "GameObjectsMetaData/GameObjectType.enum"
#undef GAMEOBJECT_TYPE
	}
}

namespace MarbleBall
{
	class MarbleBall : public Hollow::Application
	{
	public:
		MarbleBall()
		{
			Hollow::GameObjectMetaData::Instance().Init();
			// Engine Initialization
			Application::Init("Resources/Settings.json");

			Hollow::SceneManager::Instance().LoadLevel("a");
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