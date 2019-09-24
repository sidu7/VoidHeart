#include <hollowpch.h>
#include "Hollow/Core/GameObject.h"
#include "Hollow/Components/Component.h"

namespace Hollow {

	unsigned int GameObject::ID = 0;

	GameObject::GameObject(): mID(ID++), mActive(false)
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

	void GameObject::DebugDisplay()
	{
		if (ImGui::TreeNode((void*)mID, "Object %d", mID))
		{
			for (std::type_index typeIndex : mIndices)
			{
				mComponents[typeIndex]->DebugDisplay();
			}
			ImGui::TreePop();
		}
	}

}