#include <hollowpch.h>
#include "ParticleEmitter.h"

#include "Hollow/Managers/ResourceManager.h"

#include "Hollow/Graphics/Mesh.h"
#include "Hollow/Graphics/VertexArray.h"
#include "Hollow/Graphics/VertexBuffer.h"
#include "Hollow/Graphics/Texture.h"
#include "Hollow/Graphics/Data/ParticleData.h"
#include "Hollow/Graphics/ShaderStorageBuffer.h"

namespace Hollow
{
	ParticleEmitter ParticleEmitter::instance;

	void ParticleEmitter::Init()
	{
		mCenterOffset = glm::vec3(0.0f);
		mCount = 0;
		mSpeedRange = glm::vec2(0.0f);
		mLifeRange = glm::vec2(0.0f);
		mCenterOffset = glm::vec3(0.0f);
		mAreaOfEffect = glm::vec3(0.0f);
	}

	void ParticleEmitter::Clear()
	{		
		mParticlesList.clear();
		mParticlePositions.clear();
		mpParticle.clear();
		mModelMatrices.clear();
		delete mpParticlePositionVBO;
		delete mTexture;
		delete mpParticlePositionVAO;
		delete mpParticleModelVBO;
		delete mpParticleStorage;
	}

	void ParticleEmitter::Serialize(rapidjson::Value::Object data)
	{
		mCount = static_cast<unsigned long>(data["Count"].GetFloat());
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
		if (data.HasMember("Speed"))
		{
			mSpeedRange = JSONHelper::GetVec2F(data["Speed"].GetArray());
		}
		if (data.HasMember("Life"))
		{
			mLifeRange = JSONHelper::GetVec2F(data["Life"].GetArray());
		}
		if (data.HasMember("ComputeShader"))
		{
			mpComputeShader = ResourceManager::Instance().LoadShader(data["ComputeShader"].GetString());
		}
		if(data.HasMember("CenterOffset"))
		{
			mCenterOffset = JSONHelper::GetVec3F(data["CenterOffset"].GetArray());
		}
		if (data.HasMember("PixelSize"))
		{
			mPixelSize = data["PixelSize"].GetFloat();
		}
	}

	void ParticleEmitter::DebugDisplay()
	{
	}	
}