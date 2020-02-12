#pragma once
#include "Component.h"
#include<unordered_map>

namespace Hollow {
	class Shape;
	class Transform;
	class Body;


	class Collider : public Component
	{
		REGISTERCOMPONENT(Collider);
	public:
		HOLLOW_API void Init();
		HOLLOW_API void Clear();
		HOLLOW_API void DebugDisplay() override;
		HOLLOW_API void Serialize(rapidjson::Value::Object);
		HOLLOW_API void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override;
	public:
		Shape* mpShape;
		Shape* mpLocalShape;

		float mBounciness, mFriction;

		bool mIsTrigger;
		bool mHasCustomScale;
		Transform* mpTr;
		Body* mpBody;
	private:
		std::string mDShape;
		glm::vec3 mMinBounds;
		glm::vec3 mMaxBounds;
	};
}

