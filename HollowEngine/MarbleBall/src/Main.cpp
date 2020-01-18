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
#include "Hollow/Components/Body.h"
#include "Hollow/Components/Transform.h"

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

			Hollow::SceneManager::Instance().LoadLevel("particletest");

			//73,6.5,-69
			/*for (int i = 0; i < 33; ++i)
			{
				Hollow::GameObject* gobj = Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/box2.json");
				Hollow::Body* body = gobj->GetComponent<Hollow::Body>();

				body->mPosition = glm::vec3(80.0f, 6.0f, -69.6 + 0.6 * i);
				
			}

			for (int i = 0; i < 32; ++i)
			{
				Hollow::GameObject* gobj = Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/box2.json");
				Hollow::Body* body = gobj->GetComponent<Hollow::Body>();

				body->mPosition = glm::vec3(80.0f, 6.6f, -69.3 + 0.6 * i);

			}

			for (int i = 0; i < 31; ++i)
			{
				Hollow::GameObject* gobj = Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/box2.json");
				Hollow::Body* body = gobj->GetComponent<Hollow::Body>();

				body->mPosition = glm::vec3(80.0f, 7.2f, -69 + 0.6 * i);

			}

			for (int i = 0; i < 30; ++i)
			{
				Hollow::GameObject* gobj = Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/box2.json");
				Hollow::Body* body = gobj->GetComponent<Hollow::Body>();

				body->mPosition = glm::vec3(80.0f, 7.8f, -68.7 + 0.6 * i);

			}
			for (int i = 0; i < 29; ++i)
			{
				Hollow::GameObject* gobj = Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/box2.json");
				Hollow::Body* body = gobj->GetComponent<Hollow::Body>();

				body->mPosition = glm::vec3(80.0f, 8.4f, -68.4 + 0.6 * i);

			}
			for (int i = 0; i < 28; ++i)
			{
				Hollow::GameObject* gobj = Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/box2.json");
				Hollow::Body* body = gobj->GetComponent<Hollow::Body>();

				body->mPosition = glm::vec3(80.0f, 9.0f, -68.1 + 0.6 * i);

			}*/
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