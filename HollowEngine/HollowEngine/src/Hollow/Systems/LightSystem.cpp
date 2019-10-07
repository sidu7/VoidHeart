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

	void LightSystem::Update(GameObject* gameobject)
	{
		LightData light;

		Light* clight = gameobject->GetComponent<Light>();
		light.mColor = clight->mColor;
		light.mCastShadow = clight->mCastShadow;
		light.mpShadowMap = clight->mpShadowMap;

		light.mPosition = gameobject->GetComponent<Transform>()->GetPosition();

		RenderManager::Instance().mLightData.push_back(light);
	}
}