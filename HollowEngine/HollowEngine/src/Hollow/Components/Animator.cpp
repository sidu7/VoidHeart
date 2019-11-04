#include <hollowpch.h>
#include "Animator.h"

#include "Hollow/Managers/ResourceManager.h"

namespace Hollow
{
	Animator Animator::instance;

	void Animator::Init()
	{
		mLoopingAnimation = true;
	}

	void Animator::Clear()
	{
		mBones.clear();
		mAnimations.clear();
		mBoneTransformations.clear();
		mRunningState = "";
		mScaleFactor = 0.0f;
		mRunTime = 0.0;
	}

	void Animator::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("BoneData"))
		{
			std::pair<float, std::vector<Bone*>> value = ResourceManager::Instance().LoadBoneData(data["BoneData"].GetString());
			mBones = value.second;
			mScaleFactor = value.first;
		}
		if (data.HasMember("Animations"))
		{
			rapidjson::Value::Array arr = data["Animations"].GetArray();
			for (unsigned int i = 0; i < arr.Size(); ++i)
			{
				auto anim = arr[i].GetArray();
				std::string name = anim[0].GetString();
				mAnimations[name] = ResourceManager::Instance().AddAnimationData(anim[1].GetString(),name,mBones,mScaleFactor);
			}
		}
	}

	void Animator::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
	}

	void Animator::DebugDisplay()
	{
	}
}