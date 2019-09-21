#pragma once

namespace Hollow {
	class GameObject;
	class GameObjectFactory
	{
		SINGLETON(GameObjectFactory)
	public:
		GameObject* LoadObject(const char* pFileName);
		GameObject* LoadObject(rapidjson::Value::Object root, std::string fname);

	};
}