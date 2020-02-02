#pragma once
#include <Hollow.h>
#include "Hollow/Components/Component.h"

namespace BulletHell
{
	class ParentOffset : public Hollow::Component
	{
		REGISTERCOMPONENT(ParentOffset)
	public:
		void Init();
		void Serialize(rapidjson::Value::Object data);
		void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);
		void Clear();
		void DebugDisplay();

	public:
		Hollow::GameObject* mpParent;
		glm::vec3 mOffset;
		std::string mTag;
	};
}