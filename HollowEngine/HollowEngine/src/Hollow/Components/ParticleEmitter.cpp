#include <hollowpch.h>
#include "ParticleEmitter.h"

#include "Hollow/Managers/ResourceManager.h"

#include "Hollow/Graphics/Mesh.h"
#include "Hollow/Graphics/VertexArray.h"
#include "Hollow/Graphics/VertexBuffer.h"

namespace Hollow
{
	ParticleEmitter ParticleEmitter::instance;

	void ParticleEmitter::Init()
	{
		mCount = 0;
		mMinSpeed = 0.0f;
		mMaxSpeed = 0.0f;
		mCenterOffset = glm::vec3(0.0f);
		mAreaOfEffect = glm::vec3(0.0f);
	}

	void ParticleEmitter::Clear()
	{
		mCount = 0;
		mMinSpeed = 0.0f;
		mMaxSpeed = 0.0f;
		mCenterOffset = glm::vec3(0.0f);
		mAreaOfEffect = glm::vec3(0.0f);
		delete mpModelMatricesVBO;
	}

	void ParticleEmitter::Serialize(rapidjson::Value::Object data)
	{
		mCount = data["Count"].GetFloat();
		if (data.HasMember("Shape"))
		{
			mpParticle.push_back(ResourceManager::Instance().GetShape((Shapes)data["Shape"].GetUint()));
		}
		else if (data.HasMember("Model"))
		{
			mpParticle = ResourceManager::Instance().LoadModel(data["Model"].GetString());
		}
		UpdateAttributes();
		mAreaOfEffect = JSONHelper::GetVec3F(data["Area"].GetArray());
	}

	void ParticleEmitter::DebugDisplay()
	{
	}

	void ParticleEmitter::UpdateAttributes()
	{
		mParticlesList.reserve(mCount);
		mModelMatrices.reserve(mCount);

		mpModelMatricesVBO = new VertexBuffer();
		mpModelMatricesVBO->AddStreamingData(mCount * sizeof(glm::mat4));

		for (unsigned int i = 0; i < mpParticle.size(); ++i)
		{
			VertexArray* vao = mpParticle[i]->mpVAO;
			vao->Bind();
			vao->PushMatrix(4, GL_FLOAT, sizeof(glm::mat4), sizeof(glm::vec4));
		}
	}
}