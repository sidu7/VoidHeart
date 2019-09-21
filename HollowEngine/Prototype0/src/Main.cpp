#include <Hollow.h>

#include "Hollow/Core/EntryPoint.h"
#include "PrototypeComponent.h"
#include "Hollow/Core/GameObject.h"
#include "Hollow/Managers/GameObjectManager.h"
#include "Hollow/Components/TestComponent.h"
#include "Hollow/Managers/MemoryManager.h"

class Prototype0 : public Hollow::Application
{
public:
	Prototype0()
	{
		Hollow::GameObject* obj1 = Hollow::MemoryManager::Instance().NewGameObject();
		Hollow::GameObject* obj2 = Hollow::MemoryManager::Instance().NewGameObject();
		Hollow::TestComponent* test = static_cast<Hollow::TestComponent*>(Hollow::MemoryManager::Instance().NewComponent("TestComponent"));
		PrototypeComponent* proto = static_cast<PrototypeComponent*>(Hollow::MemoryManager::Instance().NewComponent("PrototypeComponent"));

		obj1->AddComponent(test);
		obj2->AddComponent(proto);
		Hollow::GameObjectManager::Instance().AddGameObject(obj1);
		Hollow::GameObjectManager::Instance().AddGameObject(obj2);
	}
	
	~Prototype0()
	{
	}
};


Hollow::Application* Hollow::CreateApplication()
{
	return new Prototype0();
}