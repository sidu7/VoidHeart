#include <Hollow.h>
#include "Score.h"

namespace MarbleBall
{
	Score Score::instance;
	
	void Score::Init()
	{
		mScore = 0;
		mScoreIncrement = 0;
	}

	void Score::Serialize(rapidjson::Value::Object data)
	{
		if(data.HasMember("ScoreIncrement"))
		{
			mScoreIncrement = data["ScoreIncrement"].GetInt();
		}
	}

	void Score::DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		Hollow::JSONHelper::Write("ScoreIncrement", mScoreIncrement, writer);
	}

	void Score::Clear()
	{
	}

	void Score::DebugDisplay()
	{
		ImGui::InputInt("Score", &mScore);
		ImGui::InputInt("ScoreIncrement", &mScoreIncrement);
	}
}
