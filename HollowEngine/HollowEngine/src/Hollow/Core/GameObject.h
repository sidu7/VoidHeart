#pragma once
#include "Hollow/Core/Core.h"

namespace Hollow {
	class Component;
	class GameObject
	{
	public:
		HOLLOW_API GameObject();
		HOLLOW_API ~GameObject();

		template <typename T>
		T* GetComponent();

		HOLLOW_API void AddComponent(Component* component);
		HOLLOW_API void Destroy();

		HOLLOW_API void DebugDisplay();

		HOLLOW_API void Deserialize(rapidjson::Writer<rapidjson::StringBuffer>& writer);
	
	public:
		std::unordered_map<std::type_index, Component*> mComponents;
		std::vector<std::type_index> mIndices;
		unsigned int mID;
		bool mActive;

	private:
		static unsigned int ID;
	};

	template <typename T>
	T* GameObject::GetComponent()
	{
		if (mComponents.find(std::type_index(typeid(T))) != mComponents.end())
		{
			return static_cast<T*>(mComponents[std::type_index(typeid(T))]);
		}
		else
		{
			return nullptr;
		}
	}
}