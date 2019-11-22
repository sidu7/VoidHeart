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
		component->mpOwner = this;
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
		for (std::type_index typeIndex : mIndices)
		{
			mComponents[typeIndex]->DebugDisplay();
		}
	}

	void GameObject::Deserialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		writer.StartObject();
		writer.Key("Components");
		writer.StartArray();
		for (unsigned int i = 0; i < mIndices.size(); ++i)
		{
			writer.StartObject();
			Component* component = mComponents[mIndices[i]];
			writer.Key("Type");
			writer.String(component->mComponentName.c_str());
			component->DeSerialize(writer);
			writer.EndObject();
		}
		writer.EndArray();
		writer.EndObject();
	}
}