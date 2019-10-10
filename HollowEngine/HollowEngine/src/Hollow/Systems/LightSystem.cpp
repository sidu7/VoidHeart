#include <hollowpch.h>
#include "LightSystem.h"

#include "Hollow/Components/Transform.h"
#include "Hollow/Components/Light.h"

#include "Hollow/Managers/RenderManager.h"

#include "Hollow/Graphics/RenderData.h"

namespace Hollow
{
	LightSystem LightSystem::instance;

	void LightSystem::AddGameObject(GameObject* object)
	{
		CheckComponents<Transform, Light>(object);
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

			light.mPosition = mGameObjects[i]->GetComponent<Transform>()->mPosition;

			RenderManager::Instance().mLightData.push_back(light);
		}
	}
}