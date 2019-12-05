#include <Hollow.h>

#include "Hollow/Common.h"

#include "Hollow/Core/EntryPoint.h"
#include "Hollow/Core/GameObject.h"

#include "Hollow/Managers/GameObjectManager.h"
#include "Hollow/Managers/MemoryManager.h"
#include "Hollow/Managers/RenderManager.h"
#include "Hollow/Managers/ResourceManager.h"
#include "Hollow/Managers/EventManager.h"
#include "Hollow/Managers/AudioManager.h"
#include "Hollow/Managers/SceneManager.h"

#include "Hollow/Components/Material.h"
#include "Hollow/Components/Transform.h"
#include "Hollow/Components/Body.h"
#include "Hollow/Components/Light.h"

#define NUM_VERT 1000000000
#define PI 3.14159f

#include "Hollow/Components/Collider.h"
#include "Hollow/Managers/InputManager.h"
#include "Hollow/Managers/PhysicsManager.h"
#include "Hollow/Managers/ScriptingManager.h"

#include "Hollow/Core/GameMetaData.h"

#include "GameMetaData/GameObjectType.h"
#include "GameMetaData/GameEventType.h"


// Quoting theCherno: "A layer basically handles events and draws stuff"
// The game layer could draw the entire 3d scene
class GameLayer : public Hollow::Layer
{
	void OnUpdate(float dt) override
	{		
		
	}

	void OnEvent(Hollow::Event& e) override
	{
		//HW_TRACE("{0}", e);
	}
};

// UI layer draws the UI
class UILayer : public Hollow::Layer
{
	void OnUpdate(float dt) override
	{

	}

	void OnEvent(Hollow::Event& e) override
	{
		// the UI layer handled the mouse button pressed event so it is not
		// propagated further to the game layer
		if(e.GetEventType() == Hollow::EventType::MouseButtonPressed)
		{
			e.Handled = true;
			//Test Event
		}
		//HW_TRACE("{0}", e);
	}
};

void Hollow::GameMetaData::Init()
{
	{
#define GAMEOBJECT_TYPE(name) mMapOfGameObjectTypes[#name] = (int)GameObjectType::name;
#include "GameMetaData/GameObjectType.enum"
#undef GAMEOBJECT_TYPE
	}

	{
#define GAME_EVENT(name) mMapOfGameEventTypes[#name] = (int)GameEventType::name;
#include "GameMetaData/GameEvents.enum"
#undef GAME_EVENT
	}
}


class Tetris : public Hollow::Application
{
public:
	Tetris() 
	{
		Hollow::GameMetaData::Instance().Init();
		Application::Init("Resources/Settings.json");
		
		PushLayer(new GameLayer());
		PushOverlay(new UILayer());

		Hollow::SceneManager::Instance().LoadLevel("tetris");
		
		/*Hollow::GameObject* go = Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Floor.json");
		int dim = 12;
		int height = 16;
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < dim; ++j) {
				for (int k = 0; k < dim; ++k) {
					Hollow::GameObject* go = Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/box2.json");
					Hollow::Transform* pB = go->GetComponent<Hollow::Transform>();
					pB->mPosition = glm::vec3(j, i , k);
					go->mActive = false;
				}
			}
		}*/
	}
	
	~Tetris()
	{
		HW_TRACE("Prototype Closing");
	}
};


Hollow::Application* Hollow::CreateApplication()
{
	return new Tetris();
}