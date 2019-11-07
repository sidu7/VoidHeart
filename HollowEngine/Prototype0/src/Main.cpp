#include <Hollow.h>

#include "PrototypeComponent.h"

#include "Hollow/Core/EntryPoint.h"
#include "Hollow/Core/GameObject.h"

#include "Hollow/Managers/GameObjectManager.h"
#include "Hollow/Managers/MemoryManager.h"
#include "Hollow/Managers/RenderManager.h"
#include "Hollow/Managers/ResourceManager.h"
#include "Hollow/Managers/EventManager.h"

#include "Hollow/Components/Material.h"
#include "Hollow/Components/Transform.h"
#include "Hollow/Components/Body.h"

#define NUM_VERT 1000000000
#define PI 3.14159f

#include "Hollow/Components/Collider.h"
#include "Hollow/Managers/InputManager.h"
#include "Hollow/Managers/PhysicsManager.h"
#include "Hollow/Managers/ScriptingManager.h"

#include "GameOver.h"


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
			Hollow::GameOverEvent test;
			Hollow::EventManager::Instance().BroadcastToSubscribers(&test);
			//Test Event
		}
		//HW_TRACE("{0}", e);
	}
};

class Prototype0 : public Hollow::Application
{
public:
	Prototype0() : Application("Resources/Settings.json")
	{
		PushLayer(new GameLayer());
		PushOverlay(new UILayer());

		Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Camera.json");
		Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/UICamera.json");
		Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/MiniMapCamera.json");
		//for(int i = 0; i < 10; ++i)
		
		Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/TempObject.json");
		Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/box4.json");
		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/NPC.json");
		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Plane.json");
		Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Cloud.json");
		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/UIElement.json");
		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/StartMoveButton.json");
		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/StopMoveButton.json");
	
		//int dim = 2;
		//int height = 5;
		//for (int i = 0; i < dim; ++i) {
		//	for (int j = 0; j < dim; ++j) {
		//		for (int k = 0; k < height; ++k) {
		//			Hollow::GameObject* go = Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/box2.json");
		//			Hollow::Body* pB = static_cast<Hollow::Body*>(go->GetComponent<Hollow::Body>());
		//			pB->mPosition = glm::vec3(2.0f * i, 2.0f * (k+1), 2.0f * j);
		//			pB->mPreviousPosition = glm::vec3(2.0f * i, 2.0f * (k+1), 2.0f * j);
		//			//physics->dAABBTree.AddCollider(static_cast<Collider*>(go->GetComponent(COLLIDER)));
		//		}
		//	}
		//}

		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Camera.json");
		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/box4.json");
		//for(int i = 0; i < 100; ++i)
		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Camera.json");
		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/box2.json");
		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/box6.json");
		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/box8.json");

		Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Floor.json");
		//Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Light.json");
	
		Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Quad.json");
		Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/LocalLight.json");

		//Hollow::ScriptingManager::Instance().lua.script_file("Resources/Scripts/test.lua");
		//TODO: Remove test json deserialize
		/*std::ofstream file("Resources/Json data/test.json");
		rapidjson::StringBuffer s;
		rapidjson::Writer<rapidjson::StringBuffer> writer(s);
		writer.StartObject();
		writer.Key("GameObjects");
		writer.StartArray();
		std::vector<Hollow::GameObject*> gos = Hollow::GameObjectManager::Instance().GetGameObjects();
		for (Hollow::GameObject* g : gos)
		{
			g->Deserialize(writer);
		}
		writer.EndArray();
		writer.EndObject();
		file.write(s.GetString(), s.GetSize());*/
		// TESTING SOUNDS
		//Hollow::ResourceManager::Instance().LoadSound("Resources/Audio/Songs/test.wav", FMOD_DEFAULT | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL);
	}
	
	~Prototype0()
	{
		HW_TRACE("Prototype Closing");
	}
};


Hollow::Application* Hollow::CreateApplication()
{
	return new Prototype0();
}