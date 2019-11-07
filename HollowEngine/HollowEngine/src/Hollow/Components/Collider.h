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
	public:
		Shape* mpShape;
		Shape* mpLocalShape;

		float coeffRestitution, coeffStaticFriction, coeffDynamicFriction;

		bool isTrigger;

		Transform* mpTr;
		Body* mpBody;

		bool isColliding;
	};
}

