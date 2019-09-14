#pragma once

#include <unordered_map>
#include <typeindex>
#include <typeinfo>

#include "../Component/Component.h"
#include "../Component/TestComponent.h"

class GameObject
{
public:
	GameObject() : mID(0)
	{
		TestComponent* pTestComponent = new TestComponent();
		AddComponent<TestComponent>(pTestComponent);
	}
	~GameObject();

	template <typename T>
	T* GetComponent()
	{
		return static_cast<T*>(mComponents[std::type_index(typeid(T))]);
	}

	template <typename T>
	void AddComponent(T* pComponent)
	{
		mComponents[std::type_index(typeid(T))] = pComponent;
	}

	std::unordered_map<std::type_index, Component*> mComponents;
	unsigned int mID;
};