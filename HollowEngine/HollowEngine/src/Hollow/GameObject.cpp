#include <hollowpch.h>
#include "GameObject.h"
#include "Components/Component.h"

namespace Hollow {

	GameObject::GameObject(): mID(0), mActive(false)
	{
	}

	GameObject::~GameObject()
	{
	}

	void GameObject::AddComponent(Component* component)
	{
		mComponents[component->GetIndex()] = component;
		mIndices.push_back(component->GetIndex());
	}

	void GameObject::Destroy()
	{
		for (unsigned int i = 0; i < mIndices.size(); ++i)
		{
			MemoryManager::Instance().DeleteComponent(mComponents[mIndices[i]]);
		}
		mIndices.clear();
		mComponents.clear();
	}

}