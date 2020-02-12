#include <Hollow.h>
#include "Movement.h"

namespace BulletHell
{
	Movement Movement::instance;

	void Movement::Init()
	{
		mMoveDebuffFactor = 1.0f;
	}

	void Movement::Serialize(rapidjson::Value::Object data)
	{
		if (data.HasMember("Script"))
		{
			mScriptPath = data["Script"].GetString();
		}
	}

	void Movement::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		Hollow::JSONHelper::Write<std::string>("Script", mScriptPath, writer);
	}

	void Movement::Clear()
	{
	}

	void Movement::DebugDisplay()
	{
		Hollow::ImGuiHelper::InputText("Script Path", mScriptPath);
	}

}