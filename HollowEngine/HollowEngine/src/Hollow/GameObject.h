#pragma once

namespace Hollow {
	class Component;
	class GameObject
	{
	public:
		GameObject();
		~GameObject();

		template <typename T>
		T* GetComponent();

		void AddComponent(Component* component);
		void Destroy();
	
	public:
		std::unordered_map<std::type_index, Component*> mComponents;
		std::vector<std::type_index> mIndices;
		unsigned int mID;
		bool mActive;
	};

	template <typename T>
	T* GameObject::GetComponent()
	{
		// Map find
		return static_cast<T*>(mComponents[std::type_index(typeid(T))]);
	}
}