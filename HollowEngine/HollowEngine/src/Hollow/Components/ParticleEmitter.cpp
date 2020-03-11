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
		mMaxCount = 0;
		mEmissionRate = 0;
		mDrawCount = 0;
		mAlpha = 1.0f;
		mFadeSpeed = 1.0f;
		
		mActive = true;
		mLighting = false;
		mNeedRotation = false;
		mModelVertexParticles = false;
		mTexture = nullptr;
		mModelMatrix = glm::mat4(1.0f);
		mpComputeShader = nullptr;
		mType = PARTICLE_NUM;
		
		mDirection = glm::vec3(0.0f);
		mSpeedRange = glm::vec2(0.0f);
		mLifeRange = glm::vec2(0.0f);
		mSizeRange = glm::vec2(0.0f);
		mCenterOffset = glm::vec3(0.0f);
		mAreaOfEffect = glm::vec3(1.0f);
		mParticleColor = glm::vec3(0.0f);
		mMinColor = glm::vec3(0.0f);
		mMaxColor = glm::vec3(0.0f);
		mSpecular = glm::vec3(1.0f);
		mExtraData = glm::vec4(0.0f);
		mShininess = 0.0f;
		mPixelSize = 0.0f;

		mDType = -1;
		mComputeShaderPath = "";
		mDTexturePath = "";
		mDModelPath = "";
		mDModelShape = "";
	}

	void ParticleEmitter::Clear()
	{		
		delete mpParticleVAO;
		delete mpParticleStorage;
	}

	void ParticleEmitter::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("MaxCount"))
		{
			mMaxCount = static_cast<unsigned long>(data["MaxCount"].GetUint64());
		}
		if(data.HasMember("EmissionRate"))
		{
			mEmissionRate = static_cast<unsigned long>(data["EmissionRate"].GetUint64());
		}
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
				if (data.HasMember("Model"))
				{
					mDModelPath = data["Model"].GetString();
					mParticleModel = ResourceManager::Instance().LoadModel(mDModelPath);
					mParticleMaterials = ResourceManager::Instance().LoadMaterials(mDModelPath);
				}
				if (data.HasMember("ModelShape"))
				{
					mDModelShape = data["ModelShape"].GetString();
					mParticleModel.push_back(ResourceManager::Instance().GetShape(mDModelShape));
				}
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
			mParticleColor = mMinColor = mMaxColor = JSONHelper::GetVec3F(data["ParticleColor"].GetArray());
		}
		if(data.HasMember("Size"))
		{
			mSizeRange = JSONHelper::GetVec2F(data["Size"].GetArray());
		}
		if (data.HasMember("Direction"))
		{
			mDirection = JSONHelper::GetVec3F(data["Direction"].GetArray());
		}
		if (data.HasMember("MinColor"))
		{
			mMinColor = JSONHelper::GetVec3F(data["MinColor"].GetArray());
		}
		if (data.HasMember("MaxColor"))
		{
			mMaxColor = JSONHelper::GetVec3F(data["MaxColor"].GetArray());
		}
		if (data.HasMember("Lighting"))
		{
			mLighting = data["Lighting"].GetBool();
		}
		if (data.HasMember("NeedRotation"))
		{
			mNeedRotation = data["NeedRotation"].GetBool();
		}
		if(data.HasMember("Specular"))
		{
			mSpecular = JSONHelper::GetVec3F(data["Specular"].GetArray());
		}
		if(data.HasMember("Shininess"))
		{
			mShininess = data["Shininess"].GetFloat();
		}
		if (data.HasMember("ExtraData"))
		{
			mExtraData = JSONHelper::GetVec4F(data["ExtraData"].GetArray());
		}
		if(data.HasMember("ModelVerticesParticles"))
		{
			mModelVertexParticles = data["ModelVerticesParticles"].GetBool();
		}
		if(data.HasMember("FadeSpeed"))
		{
			mFadeSpeed = data["FadeSpeed"].GetFloat();
		}
	}

	void ParticleEmitter::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		JSONHelper::Write("MaxCount", mMaxCount, writer);
		JSONHelper::Write("EmissionRate", mEmissionRate, writer);
		JSONHelper::Write("Direction", mDirection, writer);
		JSONHelper::Write("Shape", mDType, writer);
		JSONHelper::Write("Texture", mDTexturePath, writer);
		JSONHelper::Write("Model", mDModelPath, writer);
		JSONHelper::Write("ModelShape", mDModelShape, writer);
		JSONHelper::Write("Area", mAreaOfEffect, writer);
		JSONHelper::Write("Speed", mSpeedRange, writer);
		JSONHelper::Write("Life", mLifeRange, writer);
		JSONHelper::Write("Size", mSizeRange, writer);
		JSONHelper::Write("ComputeShader", mComputeShaderPath, writer);
		JSONHelper::Write("CenterOffset", mCenterOffset, writer);
		JSONHelper::Write("PixelSize", mPixelSize, writer);
		JSONHelper::Write("Active", mActive, writer);
		JSONHelper::Write("ParticleColor", mParticleColor, writer);
		JSONHelper::Write("MinColor", mMinColor, writer);
		JSONHelper::Write("MaxColor", mMaxColor, writer);
		JSONHelper::Write("Lighting", mLighting, writer);
		JSONHelper::Write("Specular", mSpecular, writer);
		JSONHelper::Write("Shininess", mShininess, writer);
		JSONHelper::Write("ExtraData", mExtraData, writer);
		JSONHelper::Write("NeedRotation", mNeedRotation, writer);
		JSONHelper::Write("ModelVerticesParticles", mModelVertexParticles, writer);
		JSONHelper::Write("FadeSpeed", mFadeSpeed, writer);
	}

	void ParticleEmitter::DebugDisplay()
	{
		ImGui::InputInt("MaxCount", (int*)&mMaxCount);
		ImGui::InputInt("EmissionRate", (int*)&mEmissionRate);
		ImGui::InputInt("Shape", (int*)&mDType);
		ImGui::Checkbox("Active", &mActive);
		ImGuiHelper::InputText("Texture File", mDTexturePath);
		ImGuiHelper::InputText("Model File", mDModelPath);
		ImGuiHelper::InputText("Model Shape", mDModelShape);
		ImGui::InputFloat3("Area of Effect", (float*)& mAreaOfEffect);
		ImGui::InputFloat3("Direction", &mDirection[0]);
		ImGui::InputFloat2("Speed Range", &mSpeedRange[0]);
		ImGui::InputFloat2("Life Range", &mLifeRange[0]);
		ImGui::InputFloat2("Size Range", &mSizeRange[0]);
		ImGuiHelper::InputText("Compute Shader File", mComputeShaderPath);
		ImGui::InputFloat3("Center Offset", (float*)&mCenterOffset);
		ImGui::InputFloat("PixelSize", &mPixelSize);
		ImGui::ColorEdit3("Particle Color", &mParticleColor[0], ImGuiColorEditFlags_Float);
		ImGui::ColorEdit3("Start Color", &mMinColor[0], ImGuiColorEditFlags_Float);
		ImGui::ColorEdit3("End Color", &mMaxColor[0], ImGuiColorEditFlags_Float);
		ImGui::Checkbox("Lighting", &mLighting);
		ImGui::InputFloat3("Particle Specular", (float*)&mSpecular);
		ImGui::InputFloat("Particle Shininess", &mShininess);
		ImGui::InputFloat4("Extra Data", &mExtraData[0]);
		ImGui::Checkbox("Need Rotation Matrix", &mNeedRotation);
		ImGui::Checkbox("Model Vertices Particles", &mModelVertexParticles);
		ImGui::InputFloat("Fade Speed", &mFadeSpeed);
	}	
}
