#include <hollowpch.h>
#include "ParticleEmitter.h"

#include "Hollow/Managers/ResourceManager.h"

#include "Hollow/Graphics/Mesh.h"
#include "Hollow/Graphics/VertexArray.h"
#include "Hollow/Graphics/VertexBuffer.h"
#include "Hollow/Graphics/Texture.h"
#include "Hollow/Graphics/Data/ParticleData.h"

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
		delete mpParticlePositionVBO;
		delete mTexture;
		delete mpParticlePositionVAO;
		delete mpParticleModelVBO;
	}

	void ParticleEmitter::Serialize(rapidjson::Value::Object data)
	{
		mCount = data["Count"].GetFloat();
		if (data.HasMember("Shape"))
		{
			//mpParticle.push_back(ResourceManager::Instance().GetShape((Shapes)data["Shape"].GetUint()));
			mType = (ParticleType)data["Shape"].GetUint();
			if (mType == POINT)
			{
				mTexture = new Texture(data["Texture"].GetString());
			}
			else if (mType == MODEL)
			{
				mpParticle = ResourceManager::Instance().LoadModel(data["Model"].GetString());
			}
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
		mParticlePositions.reserve(mCount);
		
		mpParticlePositionVBO = new VertexBuffer();

		mpParticlePositionVAO = new VertexArray();
		mpParticlePositionVAO->AddBuffer(*mpParticlePositionVBO);

		mpParticlePositionVBO->AddStreamingData(mCount * sizeof(glm::vec4));

		mpParticlePositionVAO->Push(4, GL_FLOAT, sizeof(float));
		mpParticlePositionVAO->AddLayout();

		mpParticlePositionVAO->Unbind();

		mpParticleModelVBO = new VertexBuffer();

		mpParticleModelVBO->AddStreamingData(mCount * sizeof(glm::mat4));

		for (unsigned int i = 0; i < mpParticle.size(); ++i)
		{
			VertexArray* vao = mpParticle[i]->mpVAO;
			vao->Bind();
			vao->PushMatrix(4, GL_FLOAT, sizeof(glm::mat4), sizeof(glm::vec4));
			vao->Unbind();
		}
	}
}