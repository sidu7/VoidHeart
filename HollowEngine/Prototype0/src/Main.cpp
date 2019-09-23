#include <Hollow.h>

#include "Hollow/Core/EntryPoint.h"
#include "PrototypeComponent.h"
#include "Hollow/Core/GameObject.h"
#include "Hollow/Managers/GameObjectManager.h"
#include "Hollow/Components/TestComponent.h"
#include "Hollow/Managers/MemoryManager.h"
#include "Hollow/Managers/ResourceManager.h"

// Quoting theCherno: "A layer basically handles events and draws stuff"
// The game layer could draw the entire 3d scene
class GameLayer : public Hollow::Layer
{
	void OnUpdate(float dt) override
	{
		
	}

	void OnEvent(Hollow::Event& e) override
	{
		HW_TRACE("{0}", e);
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
		}
		HW_TRACE("{0}", e);
	}
};

class Prototype0 : public Hollow::Application
{
public:
	Prototype0()
	{
		PushLayer(new GameLayer());
		PushOverlay(new UILayer());
		
		/*Hollow::GameObject* obj1 = Hollow::MemoryManager::Instance().NewGameObject();
		Hollow::GameObject* obj2 = Hollow::MemoryManager::Instance().NewGameObject();
		Hollow::TestComponent* test = static_cast<Hollow::TestComponent*>(Hollow::MemoryManager::Instance().NewComponent("TestComponent"));
		PrototypeComponent* proto = static_cast<PrototypeComponent*>(Hollow::MemoryManager::Instance().NewComponent("PrototypeComponent"));

		obj1->AddComponent(test);
		obj2->AddComponent(proto);
		Hollow::GameObjectManager::Instance().AddGameObject(obj1);
		Hollow::GameObjectManager::Instance().AddGameObject(obj2);*/

		Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/TempObject.json");
	}
	
	~Prototype0()
	{
	}
};


Hollow::Application* Hollow::CreateApplication()
{
	return new Prototype0();
}