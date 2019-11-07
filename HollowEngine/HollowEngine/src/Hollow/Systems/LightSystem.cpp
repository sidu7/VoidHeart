#include <hollowpch.h>
#include "LightSystem.h"

#include "Hollow/Components/Transform.h"
#include "Hollow/Components/Light.h"

#include "Hollow/Managers/RenderManager.h"

#include "Hollow/Graphics/Data/LightData.h"

namespace Hollow
{
	LightSystem LightSystem::instance;

	void LightSystem::AddGameObject(GameObject* object)
	{
		CheckAllComponents<Transform, Light>(object);
	}

	void LightSystem::Update()
	{
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			LightData light;

			Light* clight = mGameObjects[i]->GetComponent<Light>();
			light.mColor = clight->mColor;
			light.mCastShadow = clight->mCastShadow;
			light.mpShadowMap = clight->mpShadowMap;
			light.mShadowMapFarPlane = clight->mShadowMapFarPlane;
			light.mShadowMapNearPlane = clight->mShadowMapNearPlane;
			light.mRadius = clight->mRadius;
			light.mBlurDistance = clight->mBlurDistance;
			light.mAlpha = clight->mAlpha;
			light.mMD = clight->mMD;

			light.mPosition = mGameObjects[i]->GetComponent<Transform>()->mPosition;// +glm::vec3(i, 0.5, i);
			//light.mPosition = mGameObjects[i]->GetComponent<Transform>()->mPosition;

			RenderManager::Instance().mLightData.push_back(light);
		}
	}
}