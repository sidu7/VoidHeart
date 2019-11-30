#include <Hollow.h>
#include "LayerSystem.h"
#include "Hollow/Components/Transform.h"
#include "Hollow/Components/Model.h"
#include "Hollow/Managers/DebugDrawManager.h"

LayerSystem LayerSystem::instance;

void LayerSystem::Init()
{
	once = true;
}

void LayerSystem::Update()
{
	/*if (once)
	{
		for (int i = 0; i < mGameObjects.size() - 1; ++i)
		{
			mGameObjects[i + 1]->mActive = mLayers[i / 100][i % 10][(i / 10) % 10];
			Hollow::Transform* transform = mGameObjects[i + 1]->GetComponent<Hollow::Transform>();
			Hollow::DebugDrawManager::Instance().DebugCube(transform->mPosition, transform->mScale);
		}
		once = false;
	}*/
	for (int i = 0; i < mGameObjects.size() - 1; ++i)
	{
		mGameObjects[i + 1]->mActive = mLayers[i / 100][i % 10][(i / 10) % 10];
		Hollow::Transform* transform = mGameObjects[i + 1]->GetComponent<Hollow::Transform>();
		Hollow::DebugDrawManager::Instance().DebugCube(transform->mPosition, transform->mScale);
	}
}

void LayerSystem::AddGameObject(Hollow::GameObject* object)
{
	CheckAllComponents<Hollow::Transform, Hollow::Model>(object);
}
