#include "DestroyTimer.h"
#include "Hollow/Common.h"

namespace BulletHell
{

	DestroyTimer DestroyTimer::instance;

	void DestroyTimer::Init()
	{
	}

	void DestroyTimer::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("MaxTime"))
		{
			mMaxTime = data["MaxTime"].GetFloat();
		}
	}

	void DestroyTimer::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		Hollow::JSONHelper::Write("MaxTime", mMaxTime, writer);
	}

	void DestroyTimer::Clear()
	{
	}

	void DestroyTimer::DebugDisplay()
	{
		ImGui::InputFloat("Max Time", &mMaxTime);
		ImGui::Text("Current Time %f", mCurrentTime);
	}

}