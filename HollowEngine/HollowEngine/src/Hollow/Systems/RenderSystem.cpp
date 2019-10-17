#include <hollowpch.h>
#include "RenderSystem.h"

#include "Hollow/Managers/RenderManager.h"
#include "Hollow/Managers/DebugDrawManager.h"

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

		Transform* trans = mGameObjects[i]->GetComponent<Transform>();
		trans->mTransformationMatrix = glm::translate(glm::mat4(1.0f), trans->mPosition);
		trans->mTransformationMatrix *= glm::toMat4(trans->mQuaternion);
		trans->mTransformationMatrix = glm::scale(trans->mTransformationMatrix, trans->mScale);

		data.mpModel = trans->mTransformationMatrix;

		//DebugDrawManager::Instance().DebugSphere(trans->GetPosition(), glm::vec3(5.0f));
		//DebugDrawManager::Instance().DebugCircle(trans->mPosition + glm::vec3(0.0, 5.0, 0.0), glm::vec3(4.0f));
		//DebugDrawManager::Instance().DebugLine(trans->mPosition, trans->mPosition + glm::vec3(30.0f, 30.0f, -10.0f), COLOR_ORANGE);
		DebugDrawManager::Instance().DebugCube(trans->mPosition,glm::vec3(2.0f));
		DebugDrawManager::Instance().DebugAxes(trans->mPosition, glm::vec3(2.0f));
		

		if (Material * material = mGameObjects[i]->GetComponent<Material>())
		{
			data.mpMaterial = material;
		}

		Model* pModel = mGameObjects[i]->GetComponent<Model>();
		data.mpMeshes = pModel->mMeshes;

		data.mCastShadow = pModel->mCastShadow;

		RenderManager::Instance().mRenderData.push_back(data);
	}
}