#include <hollowpch.h>
#include "Animator.h"

#include "Hollow/Managers/ResourceManager.h"

namespace Hollow
{
	Animator Animator::instance;

	void Animator::Init()
	{
	}

	void Animator::Clear()
	{
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
				ResourceManager::Instance().AddAnimationData(arr[i].GetString(),mBones,mScaleFactor);
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