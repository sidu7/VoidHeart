#include <Hollow.h>

#include "PrototypeComponent.h"
#include "Utils/Random.h"

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
#include "Hollow/Components/Light.h"

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
		Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Floor.json");
		Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Camera.json");
		Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/TempObject.json");
		Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Cloud.json");
		Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/NPC.json");
		Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Plane.json");

		Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/UICamera.json");
		Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/MiniMapCamera.json");
		Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Light.json");
		Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Quad.json");

		auto randomizer = Random::Range(0.0f, 1.0f);
		/*for (int j = 0; j < 32; ++j)
		{
			for (int i = 0; i < 32; ++i)
			{
				Hollow::GameObject* go = Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/LocalLight.json");
				Hollow::Transform* pB = static_cast<Hollow::Transform*>(go->GetComponent<Hollow::Transform>());
				pB->mPosition = glm::vec3(i - 8.0, 0.70, j - 8.0);
				Hollow::Light* pL = go->GetComponent<Hollow::Light>();
				pL->mColor = glm::vec3(randomizer(), randomizer(), randomizer());
			}
		}*/

		int dim = 1;
		int height = 5;
		for (int i = 0; i < dim; ++i) {
			for (int j = 0; j < dim; ++j) {
				for (int k = 0; k < height; ++k) {
					Hollow::GameObject* go = Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/box2.json");
					Hollow::Body* pB = static_cast<Hollow::Body*>(go->GetComponent<Hollow::Body>());
					pB->mPosition = glm::vec3(2.0f * i, 2.2f * (k + 2), 2.0f * j);
					pB->mPreviousPosition = glm::vec3(2.0f * i, 2.2f * (k + 2), 2.0f * j);
				}
			}
		}
		Hollow::ResourceManager::Instance().LoadSound("Resources/Audio/Songs/test.wav", FMOD_DEFAULT | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL);
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