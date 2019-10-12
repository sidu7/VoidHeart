#include <hollowpch.h>
#include "ParticleSystem.h"

#include "Hollow/Components/ParticleEmitter.h"
#include "Hollow/Components/Transform.h"

#include "Hollow/Graphics/VertexBuffer.h"

#include "Hollow/Managers/RenderManager.h"

namespace Hollow
{
	ParticleSystem ParticleSystem::instance;

	void ParticleSystem::AddGameObject(GameObject* object)
	{
		CheckComponents<ParticleEmitter>(object);
	}
	
	void ParticleSystem::Update()
	{
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			ParticleEmitter* emitter = mGameObjects[i]->GetComponent<ParticleEmitter>();

			CalculateParticleMatrices(emitter);

			glm::mat4 model = glm::mat4(1.0f);

			model = glm::translate(model, mGameObjects[i]->GetComponent<Transform>()->mPosition);
			model = glm::scale(model, glm::vec3(10.0f));
			//emitter->mpParticlePositionVBO->AddSubData(
			//	&emitter->mParticlePositions[0], // data
			//	emitter->mCount * sizeof(glm::vec4),  // size of the parent VBO
			//	emitter->mParticlePositions.size() * sizeof(glm::vec4)); // size of data to be sent

			emitter->mpParticleModelVBO->AddSubData(
				&emitter->mModelMatrices[0],
				emitter->mCount * sizeof(glm::mat4),
				emitter->mModelMatrices.size() * sizeof(glm::mat4));

			ParticleData particle;

			particle.mParticleModel = emitter->mpParticle;
			particle.mParticlesCount = emitter->mModelMatrices.size();
			particle.mpParticleVBO = emitter->mpParticleModelVBO;
			particle.mpParticleVAO = emitter->mpParticlePositionVAO;
			particle.mModel = glm::translate(model,glm::vec3(5.0f,0.0f,0.0f));
			particle.mTex = emitter->mTexture;
			RenderManager::Instance().mParticleData.push_back(particle);
		}
	}

	void ParticleSystem::CalculateParticleMatrices(ParticleEmitter* emitter)
	{
		emitter->mParticlePositions.clear();
		emitter->mModelMatrices.clear();

		int amount = 2000;		
		float radius = 4.0;
		float offset = 2.0f;
		for (unsigned int i = 0; i < amount; i++)
		{
			/*float x, y, z;

			x = (rand() % (2 * amount + 1) - amount) / ((float)amount);
			y = (rand() % (2 * amount + 1) - amount) / ((float)amount);
			z = (rand() % (2 * amount + 1) - amount) / ((float)amount);

			emitter->mParticlePositions.push_back(glm::vec4(x, y, z, 1.0f));*/

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
}