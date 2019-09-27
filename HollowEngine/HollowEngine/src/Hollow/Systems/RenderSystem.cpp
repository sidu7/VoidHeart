#include <hollowpch.h>
#include "RenderSystem.h"

#include "Hollow/Managers/RenderManager.h"

#include "Hollow/Components/Transform.h"
#include "Hollow/Components/Model.h"
#include "Hollow/Components/Material.h"

#include "Hollow/Graphics/RenderData.h"
#include "Hollow/Graphics/Mesh.h"

Hollow::RenderSystem Hollow::RenderSystem::instance;

void Hollow::RenderSystem::AddGameObject(GameObject* object)
{
	CheckComponents<Transform,Model>(object);
}

void Hollow::RenderSystem::Update()
{
	for (unsigned int i = 0; i < mGameObjects.size(); ++i)
	{
		RenderData data;
		data.mpModel = mGameObjects[i]->GetComponent<Transform>()->GetTranformationMatrix();

		if (Material * material = mGameObjects[i]->GetComponent<Material>())
		{
			data.mpMaterial = material;
		}

		Model* pModel = mGameObjects[i]->GetComponent<Model>();
		data.mpMeshes = pModel->GetMeshes();

		RenderManager::Instance().mRenderData.push_back(data);
	}
}