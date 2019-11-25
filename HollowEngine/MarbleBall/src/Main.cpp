#include <Hollow.h>
#include "Hollow/Common.h"
#include "Hollow/Core/EntryPoint.h"

#include "Hollow/Managers/GameObjectManager.h"
#include "Hollow/Managers/MemoryManager.h"
#include "Hollow/Managers/RenderManager.h"
#include "Hollow/Managers/ResourceManager.h"
#include "Hollow/Managers/EventManager.h"
#include "Hollow/Managers/SceneManager.h"

class Prototype : public Hollow::Application
{
public:
	Prototype() : Application("Resources/Settings.json")
	{
		Hollow::SceneManager::Instance().LoadLevel("Marble");
		auto randomizer = Random::Range(0.0f, 1.0f);
	}

	~Prototype()
	{
		HW_TRACE("Prototype Closing");
	}
};


Hollow::Application* Hollow::CreateApplication()
{
	return new Prototype();
}