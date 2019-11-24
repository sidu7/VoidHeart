#include <hollowpch.h>

#include "Animator.h"

#include "Hollow/Managers/ResourceManager.h"

namespace Hollow
{
	Animator Animator::instance;

	void Animator::Init()
	{
		// public data
		mLoopingAnimation = true;
		mCurrentState = "";
		mPreviousState = "";
		mScaleFactor = 0.0f;
		mCurrentRunTime = 0.0;
		mPreviousRunTime = 0.0;
		mBlendFactor = 1.0f;
		mBlending = false;
		mBlendTime = 0.0f;

		// private data
		mBoneDataPath = "";
	}

	void Animator::Clear()
	{
		// public data
		mBones.clear();
		mAnimationData.clear();
		mAnimations.clear();
		mBoneTransformations.clear();
		mSkeleton.clear();

		// private data
		mAnimationsDataPath.clear();
	}

	void Animator::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("BoneData"))
		{
			mBoneDataPath = data["BoneData"].GetString();
			std::pair<float, std::vector<Bone*>> value = ResourceManager::Instance().LoadBoneData(mBoneDataPath);
			mBones = value.second;
			mScaleFactor = value.first;
			for(auto bone: mBones)
			{
				mAnimationData.emplace_back(std::unordered_map<std::string, std::pair<bool, AnimationData>>());
			}
		}
		if (data.HasMember("Animations"))
		{
			rapidjson::Value::Array arr = data["Animations"].GetArray();
			for (unsigned int i = 0; i < arr.Size(); ++i)
			{
				auto anim = arr[i].GetArray();
				std::string name = anim[0].GetString();
				mAnimations[name] = ResourceManager::Instance().AddAnimationData(anim[1].GetString(),name,mBones,mAnimationData,mScaleFactor);
				mAnimationsDataPath.emplace_back(std::make_pair(name, anim[1].GetString()));
			}
		}
		if(data.HasMember("BlendFactor"))
		{
			mBlendFactor = data["BlendFactor"].GetFloat();
		}
	}

	void Animator::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		JSONHelper::Write<std::string>("BoneData",mBoneDataPath,writer);
		writer.Key("Animations");
		writer.StartArray();
		for(auto anim : mAnimationsDataPath)
		{
			writer.StartArray();
			writer.String(anim.first.c_str());
			writer.String(anim.second.c_str());
			writer.EndArray();
		}
		writer.EndArray();
		JSONHelper::Write("BlendFactor", mBlendFactor, writer);
	}

	void Animator::DebugDisplay()
	{
		ImGui::InputFloat("BlendFactor", &mBlendFactor);
	}
}