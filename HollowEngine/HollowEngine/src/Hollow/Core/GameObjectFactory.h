#pragma once
#include "Hollow/Common.h"

namespace Hollow {
	class GameObject;
	class GameObjectFactory
	{
		SINGLETON(GameObjectFactory)
	public:
		/// <summary>
		/// Creates and Serializes the GameObject from the JSON and returns its pointer.
		/// </summary>
		/// <param name="data">JSON root.</param>
		HOLLOW_API GameObject* LoadObject(rapidjson::Value::Object root);
	};
}