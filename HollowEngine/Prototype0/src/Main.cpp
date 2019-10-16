#include <Hollow.h>

#include "PrototypeComponent.h"

#include "Hollow/Core/EntryPoint.h"
#include "Hollow/Core/GameObject.h"

#include "Hollow/Managers/GameObjectManager.h"
#include "Hollow/Managers/MemoryManager.h"
#include "Hollow/Managers/RenderManager.h"
#include "Hollow/Managers/ResourceManager.h"

#include "Hollow/Components/Material.h"
#include "Hollow/Components/Transform.h"

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
		}
		//HW_TRACE("{0}", e);
	}
};

class Prototype0 : public Hollow::Application
{
public:
	Prototype0()
	{
		PushLayer(new GameLayer());
		PushOverlay(new UILayer());
		
		//for(int i = 0; i < 100; ++i)
		Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Camera.json");
		Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/TempObject.json");
		Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Floor.json");
		Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json data/Light.json");
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