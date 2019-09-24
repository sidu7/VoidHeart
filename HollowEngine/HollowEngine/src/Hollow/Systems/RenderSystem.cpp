#include <hollowpch.h>
#include "RenderSystem.h"
#include "Hollow/Components/Transform.h"
#include "Hollow/Components/Model.h"

void Hollow::RenderSystem::AddGameObject(GameObject* object)
{
	CheckComponents<Transform,Model>(object);
}

void Hollow::RenderSystem::Update()
{
	for (unsigned int i = 0; i < mGameObjects.size(); ++i)
	{
		Model* pModel = mGameObjects[i]->GetComponent<Model>();
		for (Mesh* mesh : pModel->GetMeshes())
		{
			//TODO: do something
		}
	}
}