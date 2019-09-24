#pragma once

namespace Hollow {
	class Component;
	class HOLLOW_API GameObject
	{
	public:
		GameObject();
		~GameObject();

		template <typename T>
		T* GetComponent();

		void AddComponent(Component* component);
		void Destroy();

		void DebugDisplay();
	
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