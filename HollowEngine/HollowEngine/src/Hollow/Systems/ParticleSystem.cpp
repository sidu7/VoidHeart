#include <hollowpch.h>
#include "Hollow/Common.h"
#include "ParticleSystem.h"

#include "Hollow/Components/ParticleEmitter.h"
#include "Hollow/Components/Transform.h"

#include "Hollow/Graphics/VertexBuffer.h"
#include "Hollow/Graphics/VertexArray.h"
#include "Hollow/Graphics/Mesh.h"
#include "Hollow/Graphics/ShaderStorageBuffer.h"
#include "Hollow/Graphics/Data/ParticleData.h"

#include "Hollow/Managers/RenderManager.h"
#include "Hollow/Managers/FrameRateController.h"


namespace Hollow
{
	ParticleSystem ParticleSystem::instance;

	void ParticleSystem::AddGameObject(GameObject* object)
	{
		if (CheckAllComponents<ParticleEmitter>(object))
		{
			UpdateAttributes(object->GetComponent<ParticleEmitter>());
		}
	}
	
	void ParticleSystem::Update()
	{	
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			ParticleEmitter* emitter = mGameObjects[i]->GetComponent<ParticleEmitter>();

			float fps = 1.0f / FrameRateController::Instance().GetFrameTime();
			float deltaParticles = emitter->mEmissionRate / fps;		
			if (emitter->mDrawCount + deltaParticles < emitter->mMaxCount)
			{
				emitter->mDrawCount += deltaParticles;
			}
			if(emitter->mActive)
			{
				emitter->mCount = emitter->mDrawCount;
			}
			else
			{
				emitter->mDrawCount = 0;
			}

			Transform* transform = mGameObjects[i]->GetComponent<Transform>();
			ParticleData particle;
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, transform->mPosition + emitter->mCenterOffset);
			emitter->mModelMatrix = glm::scale(model, emitter->mAreaOfEffect);
			particle.emitter = emitter;
			
			RenderManager::Instance().mParticleData.push_back(particle);
		}
	}

	void ParticleSystem::UpdateAttributes(ParticleEmitter* emitter)
	{					
		emitter->mpParticleStorage = new ShaderStorageBuffer();
		emitter->mpParticleStorage->CreateBuffer(emitter->mMaxCount * sizeof(Particle));
		Particle* particles = static_cast<Particle*>(emitter->mpParticleStorage->GetBufferWritePointer(true));

		auto randomizer = Random::Range(-1.0f,1.0f);

		for (unsigned int i = 0; i < emitter->mMaxCount; ++i)
		{
			float x = randomizer(); float y = randomizer(); float z = randomizer();
			particles[i].mPosition = glm::vec3(x,y,z);			
			particles[i].mSpeed = 0.0f;
			particles[i].mLife = 0.0f;
			particles[i].mCurrentLife = 0.0f;
			particles[i].mDirection = glm::vec3(0.0f);
		}

		emitter->mpParticleStorage->ReleaseBufferPointer();
		emitter->mpParticleVAO = new VertexArray();					
	}
}