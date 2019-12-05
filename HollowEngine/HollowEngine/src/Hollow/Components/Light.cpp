#include <hollowpch.h>
#include "Light.h"

#include "Hollow/Graphics/FrameBuffer.h"

namespace Hollow
{
	Light Light::instance;

	void Light::Init()
	{
		mPosition = glm::vec3(0.0f);
		mRadius = 0.0f;
		mColor = glm::vec3(0.0f);
		mType = LIGHT_NUM;

		mCastShadow = false;
		mpShadowMap = nullptr;
		mShadowMapSize = glm::vec2(0.0f);
		mShadowMapNearPlane = 0.0f;
		mShadowMapFarPlane = 0.0f;
		mLookAtPoint = glm::vec3(0.0f);
		mFOV = 45.0f;

		mBlurDistance = 0;
		mAlpha = 0.0f;
		mMD = 0.0f;
	}

	void Light::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("Color"))
		{
			mColor = JSONHelper::GetVec3F(data["Color"].GetArray());
		}
		if (data.HasMember("CastShadow"))
		{
			mCastShadow = data["CastShadow"].GetBool();
			if (mCastShadow)
			{
				if (data.HasMember("ShadowMapSize"))
				{
					mShadowMapSize = JSONHelper::GetVec2F(data["ShadowMapSize"].GetArray());
					mpShadowMap = new FrameBuffer(static_cast<int>(mShadowMapSize.x), static_cast<int>(mShadowMapSize.y), 1, true);
				}
				if (data.HasMember("ShadowMapNearPlane"))
				{
					mShadowMapNearPlane = data["ShadowMapNearPlane"].GetFloat();
				}
				if (data.HasMember("ShadowMapFarPlane"))
				{
					mShadowMapFarPlane = data["ShadowMapFarPlane"].GetFloat();
				}
				if (data.HasMember("BlurDistance"))
				{
					mBlurDistance = data["BlurDistance"].GetUint();
				}
				if (data.HasMember("Alpha"))
				{
					mAlpha = data["Alpha"].GetFloat();
				}
				if (data.HasMember("MD"))
				{
					mMD = data["MD"].GetFloat();
				}
			}
		}
		if (data.HasMember("Radius"))
		{
			mRadius = data["Radius"].GetFloat();
		}
		if(data.HasMember("LookAtPoint"))
		{
			mLookAtPoint = JSONHelper::GetVec3F(data["LookAtPoint"].GetArray());
		}
		if(data.HasMember("FOV"))
		{
			mFOV = data["FOV"].GetFloat();
		}
	}

	void Light::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		JSONHelper::Write<glm::vec3>("Color", mColor, writer);
		JSONHelper::Write<bool>("CastShadow", mCastShadow, writer);
		if (mCastShadow)
		{
			JSONHelper::Write<glm::vec2>("ShadowMapSize", mShadowMapSize, writer);
			JSONHelper::Write("ShadowMapNearPlane", mShadowMapNearPlane, writer);
			JSONHelper::Write("ShadowMapFarPlane", mShadowMapFarPlane, writer);
			JSONHelper::Write("BlurDistance", mBlurDistance, writer);
			JSONHelper::Write("Alpha", mAlpha, writer);
			JSONHelper::Write("MD", mMD, writer);
			JSONHelper::Write("LookAtPoint", mLookAtPoint,writer);
			JSONHelper::Write("FOV", mFOV, writer);
		}
		JSONHelper::Write<float>("Radius", mRadius, writer);
	}

	void Light::Clear()
	{
		delete mpShadowMap;
	}

	void Light::DebugDisplay()
	{
		ImGui::ColorPicker3("Color", &mColor[0]);
		ImGui::InputFloat("Radius", &mRadius);
		ImGui::InputFloat3("Position", &mPosition[0]);

		if (mCastShadow)
		{
			ImGui::InputFloat("Shadow Map Far Plane", &mShadowMapFarPlane);
			ImGui::InputFloat("Shadow Map Near Plane", &mShadowMapNearPlane);
			ImGui::InputScalar("Blur Distance", ImGuiDataType_U32 ,&mBlurDistance);
			ImGui::InputFloat("Alpha", &mAlpha);
			ImGui::InputFloat("MD", &mMD);
			ImGui::InputFloat3("Look At Point", &mLookAtPoint[0]);
			ImGui::InputFloat("FOV", &mFOV);
		}
	}
}