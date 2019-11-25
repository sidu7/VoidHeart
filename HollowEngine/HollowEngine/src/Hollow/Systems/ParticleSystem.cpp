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
			Transform* transform = mGameObjects[i]->GetComponent<Transform>();
			ParticleData particle;
			if (emitter->mType == POINT)
			{	
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, transform->mPosition + emitter->mCenterOffset);
				emitter->mModelMatrix = glm::scale(model, emitter->mAreaOfEffect);
				
				particle.mType = POINT;
				particle.mpParticleVAO = emitter->mpParticlePositionVAO;
				particle.mCenter = transform->mPosition;
				particle.mModel = emitter->mModelMatrix;
				particle.mParticlesCount = emitter->mCount;
				particle.mpParticleDataStorage = emitter->mpParticleStorage;
				particle.mLifeRange = emitter->mLifeRange;
				particle.mSpeedRange = emitter->mSpeedRange;
				particle.mTex = emitter->mTexture;
				particle.mpComputeShader = emitter->mpComputeShader;
				particle.mPixelSize = emitter->mPixelSize;
			}
			
			RenderManager::Instance().mParticleData.push_back(particle);
		}
	}

	void ParticleSystem::UpdateAttributes(ParticleEmitter* emitter)
	{

		if (emitter->mType == ParticleType::POINT)
		{			
			emitter->mpParticleStorage = new ShaderStorageBuffer();
			emitter->mpParticleStorage->CreateBuffer(emitter->mCount * sizeof(Particle));
			Particle* particles = static_cast<Particle*>(emitter->mpParticleStorage->GetBufferWritePointer(true));

			auto randomizer = Random::Range(-1.0f,1.0f);

			for (unsigned int i = 0; i < emitter->mCount; ++i)
			{
				float x = randomizer(); float y = randomizer(); float z = randomizer();
				particles[i].mPosition = glm::vec3(x,y,z);			
				particles[i].mSpeed = 0.0f;
				particles[i].mLife = 0.0f;
				particles[i].mCurrentLife = 0.0f;
				particles[i].mPadding = glm::vec2(0.0f);
			}

			emitter->mpParticleStorage->ReleaseBufferPointer();
			emitter->mpParticlePositionVAO = new VertexArray();			
		}
	}
}