#include <hollowpch.h>
#include "RenderSystem.h"

#include "Hollow/Managers/RenderManager.h"
#include "Hollow/Managers/DebugDrawManager.h"

#include "Hollow/Components/Transform.h"
#include "Hollow/Components/Model.h"
#include "Hollow/Components/Animator.h"
#include "Hollow/Components/Material.h"

#include "Hollow/Graphics/Data/RenderData.h"
#include "Hollow/Graphics/Mesh.h"
#include "Hollow/Components/Collider.h"
#include "Hollow/Physics/Broadphase/Shape.h"

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
		Model* pModel = mGameObjects[i]->GetComponent<Model>();

		if (trans->dirtyBit) {
			glm::vec3 position = trans->mPosition;
			if(pModel->mModelHasOffset)
			{
				position -= glm::vec3(0.0f, trans->mScale.y / 2, 0.0f);
			}
			trans->mTransformationMatrix = glm::translate(glm::mat4(1.0f), position);
			trans->mTransformationMatrix *= glm::toMat4(trans->mQuaternion);
			trans->mTransformationMatrix = glm::scale(trans->mTransformationMatrix, trans->mScale);
			trans->dirtyBit = false;
		}
		data.mpModel = trans->mTransformationMatrix;

		//DebugDrawManager::Instance().DebugSphere(trans->GetPosition(), glm::vec3(5.0f));
		//DebugDrawManager::Instance().DebugCircle(trans->mPosition + glm::vec3(0.0, 5.0, 0.0), glm::vec3(4.0f));
		//DebugDrawManager::Instance().DebugLine(trans->mPosition, trans->mPosition + glm::vec3(30.0f, 30.0f, -10.0f), COLOR_ORANGE);
		//DebugDrawManager::Instance().DebugCube(trans->mPosition+ glm::vec3(0.0f,trans->mScale.x,0.0f),glm::vec3(1.0f));
		if(mGameObjects[i]->GetComponent<Collider>())
			DebugDrawManager::Instance().DebugCube(trans->mPosition,mGameObjects[i]->GetComponent<Collider>()->mpShape->GetHalfExtents() * 2.0f);
		//DebugDrawManager::Instance().DebugAxes(trans->mPosition, glm::vec3(2.0f));
		

		if (Material * material = mGameObjects[i]->GetComponent<Material>())
		{
			data.mpMaterial = material;
		}

		if (Animator* animator = mGameObjects[i]->GetComponent<Animator>())
		{
			data.mBoneTransforms = animator->mBoneTransformations;
			data.mIsAnimated = true;
		}
		else
		{
			data.mIsAnimated = false;
		}

		data.mpMeshes = pModel->mMeshes;

		data.mCastShadow = pModel->mCastShadow;

		RenderManager::Instance().mRenderData.push_back(data);
	}
}