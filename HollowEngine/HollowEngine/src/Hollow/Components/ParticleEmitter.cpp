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
		mModelMatrix = glm::mat4(1.0f);
	}

	void ParticleEmitter::Clear()
	{
		mCount = 0;
		mMinSpeed = 0.0f;
		mMaxSpeed = 0.0f;
		mCenterOffset = glm::vec3(0.0f);
		mAreaOfEffect = glm::vec3(0.0f);
		mParticlesList.clear();
		mParticlePositions.clear();
		mpParticle.clear();
		mModelMatrices.clear();
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
				mTexture = ResourceManager::Instance().LoadTexture(data["Texture"].GetString());
			}
			else if (mType == MODEL)
			{
				mpParticle = ResourceManager::Instance().LoadModel(data["Model"].GetString());
			}
		}
		if (data.HasMember("Area"))
		{
			mAreaOfEffect = JSONHelper::GetVec3F(data["Area"].GetArray());
		}
	}

	void ParticleEmitter::DebugDisplay()
	{
	}	
}