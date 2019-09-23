#pragma once

namespace Hollow {
	class GameObject;
	class GameObjectFactory
	{
		SINGLETON(GameObjectFactory)
	public:
		GameObject* LoadObject(rapidjson::Value::Object root);
	};
}