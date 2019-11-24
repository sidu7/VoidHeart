#pragma once
#include "Hollow/Common.h"

namespace Hollow {
	class GameObject;
	class GameObjectFactory
	{
		SINGLETON(GameObjectFactory)
	public:
		HOLLOW_API GameObject* LoadObject(rapidjson::Value::Object root);
	};
}