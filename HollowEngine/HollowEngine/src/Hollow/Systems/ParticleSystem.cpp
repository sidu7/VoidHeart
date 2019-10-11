#include <hollowpch.h>
#include "ParticleSystem.h"

#include "Hollow/Components/ParticleEmitter.h"

namespace Hollow
{
	void ParticleSystem::AddGameObject(GameObject* object)
	{
		CheckComponents<ParticleEmitter>(object);
	}

	void ParticleSystem::Update()
	{
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			ParticleEmitter* emitter = mGameObjects[i]->GetComponent<ParticleEmitter>();

		}
	}
}