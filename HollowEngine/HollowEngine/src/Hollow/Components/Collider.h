#pragma once
#include "Component.h"
#include<map>

namespace Hollow {
	class Shape;
	class Transform;
	class Body;

	class HOLLOW_API Collider : public Component
	{
		REGISTERCOMPONENT(Collider);
	public:
		void Init();
		void Clear();
		void DebugDisplay() override;

		void Serialize(rapidjson::Value::Object);
	public:
		Shape* mpShape;
		Shape* mpLocalShape;

		float coeffRestitution, coeffStaticFriction, coeffDynamicFriction;

		bool isTrigger;

		Transform* mpTr;
		Body* mpBody;

		bool isColliding;
	};
};

