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
		if (data.HasMember("AnimationData"))
		{
			mBones = ResourceManager::Instance().LoadBoneData(data["AnimationData"].GetString());
		}
	}

	void Animator::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
	}

	void Animator::DebugDisplay()
	{
	}
}