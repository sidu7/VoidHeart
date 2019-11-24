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
		mCount = 0;
		mTexture = nullptr;
		mModelMatrix = glm::mat4(1.0f);
		mpComputeShader = nullptr;
		mType = PARTICLE_NUM;
		
		mSpeedRange = glm::vec2(0.0f);
		mLifeRange = glm::vec2(0.0f);
		mCenterOffset = glm::vec3(0.0f);
		mAreaOfEffect = glm::vec3(0.0f);
		mPixelSize = 0.0f;

		mDType = -1;
		mComputeShaderPath = "";
		mDTexturePath = "";
	}

	void ParticleEmitter::Clear()
	{		
		delete mpParticlePositionVAO;
		delete mpParticleStorage;
	}

	void ParticleEmitter::Serialize(rapidjson::Value::Object data)
	{
		mCount = static_cast<unsigned long>(data["Count"].GetUint64());
		if (data.HasMember("Shape"))
		{
			mDType = data["Shape"].GetUint();
			mType = (ParticleType)mDType;
			if (mType == POINT)
			{
				mDTexturePath = data["Texture"].GetString();
				mTexture = ResourceManager::Instance().LoadTexture(mDTexturePath);
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
			mComputeShaderPath = data["ComputeShader"].GetString();
			mpComputeShader = ResourceManager::Instance().LoadShader(mComputeShaderPath);
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

	void ParticleEmitter::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		JSONHelper::Write("Count", mCount, writer);
		JSONHelper::Write("Shape", mDType, writer);
		JSONHelper::Write("Texture", mDTexturePath, writer);
		JSONHelper::Write("Area", mAreaOfEffect, writer);
		JSONHelper::Write("Speed", mSpeedRange, writer);
		JSONHelper::Write("Life", mLifeRange, writer);
		JSONHelper::Write("ComputeShader", mComputeShaderPath, writer);
		JSONHelper::Write("CenterOffset", mCenterOffset, writer);
		JSONHelper::Write("PixelSize", mPixelSize, writer);
	}

	void ParticleEmitter::DebugDisplay()
	{
	}	
}