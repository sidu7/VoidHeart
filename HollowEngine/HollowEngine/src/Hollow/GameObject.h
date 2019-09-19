#pragma once
#include "hollowpch.h"

namespace Hollow {
	class Component;
	class GameObject
	{
	public:
		GameObject();
		~GameObject();

		template <typename T>
		T* GetComponent();
	
		template <typename T>
		void AddComponent(T* pComponent);

		template <typename T>
		void AddComponent(T* pComponent, std::type_info index);

	public:
		std::unordered_map<std::type_index, Component*> mComponents;
		unsigned int mID;
		bool mActive;
	};

	template <typename T>
	T* GameObject::GetComponent()
	{
		// Map find
		return static_cast<T*>(mComponents[std::type_index(typeid(T))]);
	}

	template <typename T>
	void GameObject::AddComponent(T* pComponent)
	{
		mComponents[std::type_index(typeid(T))] = pComponent;
	}

	template <typename T>
	void GameObject::AddComponent(T* pComponent,std::type_info index)
	{
		mComponents[std::type_index(index)] = pComponent;
	}
}