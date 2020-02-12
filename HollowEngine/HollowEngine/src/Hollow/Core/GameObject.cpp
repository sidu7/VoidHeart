#include <hollowpch.h>
#include "Hollow/Core/GameObject.h"
#include "Hollow/Components/Component.h"
#include "Hollow/Utils/ImGuiHelper.h"

namespace Hollow {

	unsigned int GameObject::ID = 0;

	GameObject::GameObject(): mID(ID++), mActive(false), mTag("")
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
		ImGuiHelper::InputText("Type", mDType);
		ImGui::Checkbox("Active", &mActive);
		for (std::type_index typeIndex : mIndices)
		{
			mComponents[typeIndex]->ShowDebug();
		}
	}

	void GameObject::Deserialize(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		writer.StartObject();
		writer.Key("Type");
		writer.String(mDType.c_str());
		writer.Key("Active");
		writer.Bool(mActive);
		writer.Key("Tag");
		writer.String(mTag.c_str());
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