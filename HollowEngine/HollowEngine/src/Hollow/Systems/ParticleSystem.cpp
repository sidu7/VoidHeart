#include <hollowpch.h>
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

#include "Utils/Random.h"

namespace Hollow
{
	ParticleSystem ParticleSystem::instance;

	void ParticleSystem::AddGameObject(GameObject* object)
	{
		if (CheckComponents<ParticleEmitter>(object))
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
				if (transform->dirtyBit)
				{
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, transform->mPosition);
					emitter->mModelMatrix = glm::scale(model, emitter->mAreaOfEffect);
				}
				particle.mType = POINT;
				particle.mpParticleVAO = emitter->mpParticlePositionVAO;
				particle.mCenter = transform->mPosition;
				particle.mModel = emitter->mModelMatrix;
				particle.mParticlesCount = emitter->mCount;
				particle.mpShaderStorage = emitter->mShaderStorage;
				particle.mTex = emitter->mTexture;
			}
				// Create ParticleData
			if (emitter->mType == MODEL)
			{
				CalculateParticleMatrices(emitter);
			
				// Create ParticleData
				particle.mType = MODEL;
				particle.mParticleModel = emitter->mpParticle;
				particle.mParticlesCount = emitter->mModelMatrices.size();
				particle.mpParticleVBO = emitter->mpParticleModelVBO;
				particle.mParticleModelMatrices = emitter->mModelMatrices;
			}

			RenderManager::Instance().mParticleData.push_back(particle);
		}
	}

	void ParticleSystem::CalculateParticleMatrices(ParticleEmitter* emitter)
	{
		emitter->mModelMatrices.clear();

		int amount = 200;		
		float radius = 4.0;
		float offset = 2.0f;
		for (unsigned int i = 0; i < amount; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			// 1. translation: displace along circle with 'radius' in range [-offset, offset]
			float angle = (float)i / (float)amount * 360.0f;
			float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float x = sin(angle) * radius + displacement;
			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float z = cos(angle) * radius + displacement;
			model = glm::translate(model, glm::vec3(x, y, z));

			// 2. scale: Scale between 0.05 and 0.25f
			float scale = (rand() % 20) / 100.0f + 0.05;
			//model = glm::scale(model, glm::vec3(scale));
			model = glm::scale(model, glm::vec3(0.1f));

			// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
			float rotAngle = (rand() % 360);
			model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

			//model = glm::translate(model, glm::vec3(0.0f, i * 2.0f, 0.0f));
			emitter->mModelMatrices.push_back(model);
		}
	}
	void ParticleSystem::CalculateParticlePositions(ParticleEmitter* emitter, glm::vec3 center)
	{ 
		/*emitter->mParticlePositions.clear();

		float deltaTime = FrameRateController::Instance().GetFrameTime();

		//UpdateCurrentParticles
		for (unsigned int i = 0; i < emitter->mParticlesList.size(); ++i)
		{
			Particle& particle = emitter->mParticlesList[i];

			particle.mLife -= deltaTime;
			if (particle.mLife > 0.0f)
			{
				//particle.mPosition += particle.mSpeed * deltaTime * particle.mDirection;
				emitter->mParticlePositions.push_back(glm::vec4(particle.mPosition,1.0f));
			}
			else
			{
				emitter->mParticlesList.erase(emitter->mParticlesList.begin() + i);
			}
		}
		
		//SpawnNewParticles

		int amount = 200;
		for (unsigned int i = 0; i < amount; i++)
		{
			if (emitter->mParticlesList.size() >= emitter->mCount)
			{
				break;
			}
			float x, y, z;

			x = (rand() % (2 * amount + 1) - amount) / ((float)amount);
			y = (rand() % (2 * amount + 1) - amount) / ((float)amount);
			z = (rand() % (2 * amount + 1) - amount) / ((float)amount);

			Particle particle;

			particle.mPosition = glm::vec3(x, y, z);
			particle.mSpeed = 0.5f;
			particle.mDirection = glm::normalize(particle.mPosition - center);
			particle.mLife = 0.5f;

			emitter->mParticlesList.push_back(particle);
			emitter->mParticlePositions.push_back(glm::vec4(particle.mPosition, 1.0f));
		}*/
	}

	void ParticleSystem::UpdateAttributes(ParticleEmitter* emitter)
	{

		if (emitter->mType == ParticleType::POINT)
		{			
			emitter->mShaderStorage = new ShaderStorageBuffer();
			emitter->mShaderStorage->CreateBuffer(emitter->mCount * sizeof(Particle));
			Particle* particles = static_cast<Particle*>(emitter->mShaderStorage->GetBufferWritePointer());

			auto randomizer = Random::Range(-1.0f,1.0f);

			for (unsigned int i = 0; i < emitter->mCount; ++i)
			{
				particles[i].mPosition = glm::vec3(randomizer(),randomizer(),randomizer());
				particles[i].mSpeed = 0.3f;
			}

			emitter->mShaderStorage->ReleaseBufferPointer();
			emitter->mpParticlePositionVAO = new VertexArray();			
		}
		else if (emitter->mType == ParticleType::MODEL)
		{
			emitter->mpParticleModelVBO = new VertexBuffer();

			emitter->mpParticleModelVBO->AddStreamingData(emitter->mCount * sizeof(glm::mat4));

			for (unsigned int i = 0; i < emitter->mpParticle.size(); ++i)
			{
				VertexArray* vao = emitter->mpParticle[i]->mpVAO;
				vao->Bind();
				vao->PushMatrix(4, GL_FLOAT, sizeof(glm::mat4), sizeof(glm::vec4));
				vao->Unbind();
			}
		}
	}
}