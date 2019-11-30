#include <Hollow.h>
#include "Hollow/Common.h"
#include "Hollow/Core/EntryPoint.h"

#include "Hollow/Managers/GameObjectManager.h"
#include "Hollow/Managers/MemoryManager.h"
#include "Hollow/Managers/RenderManager.h"
#include "Hollow/Managers/ResourceManager.h"
#include "Hollow/Managers/EventManager.h"

class Prototype : public Hollow::Application
{
public:
	Prototype()
	{
		Application::Init("Resources/Settings.json");
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

		/*int dim = 1;
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
		}*/
		//Hollow::ResourceManager::Instance().LoadSound("Resources/Audio/Songs/test.wav", FMOD_DEFAULT | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL);
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