#pragma once

#include "hollowpch.h"
#include "Components/TestComponent.h"

class GameObject
{
public:
	GameObject() : mID(0),mActive(true)
	{
	}
	~GameObject();

	template <typename T>
	T* GetComponent()
	{
		// Map find
		return static_cast<T*>(mComponents[std::type_index(typeid(T))]);
	}

	template <typename T>
	void AddComponent(T* pComponent)
	{
		mComponents[std::type_index(typeid(T))] = pComponent;
	}

public:
	std::unordered_map<std::type_index, Component*> mComponents;
	unsigned int mID;
	bool mActive;
};