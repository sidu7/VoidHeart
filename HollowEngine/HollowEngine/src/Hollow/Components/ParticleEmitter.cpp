#include <hollowpch.h>
#include "ParticleEmitter.h"

#include "Hollow/Managers/ResourceManager.h"

#include "Hollow/Graphics/Mesh.h"
#include "Hollow/Graphics/VertexArray.h"
#include "Hollow/Graphics/VertexBuffer.h"
#include "Hollow/Graphics/Texture.h"
#include "Hollow/Graphics/Data/ParticleData.h"
#include "Hollow/Graphics/ShaderStorageBuffer.h"
#include "Hollow/Utils/ImGuiHelper.h"

namespace Hollow
{
	ParticleEmitter ParticleEmitter::instance;

	void ParticleEmitter::Init()
	{
		mCount = 0;
		mActive = true;
		mTexture = nullptr;
		mModelMatrix = glm::mat4(1.0f);
		mpComputeShader = nullptr;
		mType = PARTICLE_NUM;
		
		mSpeedRange = glm::vec2(0.0f);
		mLifeRange = glm::vec2(0.0f);
		mCenterOffset = glm::vec3(0.0f);
		mAreaOfEffect = glm::vec3(0.0f);
		mParticleColor = glm::vec3(0.0f);
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
			else if (mType == MODEL)
			{
				mDModelPath = data["Model"].GetString();
				mParticleModel = ResourceManager::Instance().LoadModel(mDModelPath);
				mParticleMaterials = ResourceManager::Instance().LoadMaterials(mDModelPath);
			}
		}
		if (data.HasMember("Active"))
		{
			mActive = data["Active"].GetBool();
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
		if (data.HasMember("ParticleColor"))
		{
			mParticleColor = JSONHelper::GetVec3F(data["ParticleColor"].GetArray());
		}
	}

	void ParticleEmitter::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		JSONHelper::Write("Count", mCount, writer);
		JSONHelper::Write("Shape", mDType, writer);
		JSONHelper::Write("Texture", mDTexturePath, writer);
		JSONHelper::Write("Model", mDModelPath, writer);
		JSONHelper::Write("Area", mAreaOfEffect, writer);
		JSONHelper::Write("Speed", mSpeedRange, writer);
		JSONHelper::Write("Life", mLifeRange, writer);
		JSONHelper::Write("ComputeShader", mComputeShaderPath, writer);
		JSONHelper::Write("CenterOffset", mCenterOffset, writer);
		JSONHelper::Write("PixelSize", mPixelSize, writer);
		JSONHelper::Write("ParticleColor", mParticleColor, writer);
	}

	void ParticleEmitter::DebugDisplay()
	{
		ImGui::InputInt("Count", (int*)&mCount);
		ImGui::InputInt("Shape", (int*)&mDType);
		ImGuiHelper::InputText("Texture File", mDTexturePath);
		ImGuiHelper::InputText("Model File", mDModelPath);
		ImGui::InputFloat3("Area of Effect", (float*)&mAreaOfEffect);
		ImGui::InputFloat2("Speed Range", (float*)&mSpeedRange);
		ImGui::InputFloat2("Life Range", (float*)&mLifeRange);
		ImGuiHelper::InputText("Compute Shader File", mComputeShaderPath);
		ImGui::InputFloat3("Center Offset", (float*)&mCenterOffset);
		ImGui::InputFloat("PixelSize", &mPixelSize);
		ImGui::ColorPicker3("Particle Color", &mParticleColor[0]);
	}	
}
