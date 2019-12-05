#pragma once
#include "Hollow/Components/Component.h"

namespace MarbleBall
{
	class Score : public Hollow::Component
	{
		REGISTERCOMPONENT(Score);
	public:
		~Score() override {}
		void Init() override;
		void Serialize(rapidjson::Value::Object data) override;
		void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;
		void Clear() override;
		void DebugDisplay() override;

	public:
		int mScore;
		int mScoreIncrement;
		bool mGoalReached;
	};
}