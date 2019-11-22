#pragma once
#include "Hollow/Managers/MemoryManager.h"

namespace Hollow
{
	class Component
	{
		friend class MemoryManager;
	public:
		Component(std::string name) {
			mComponentName = name;
		}
		Component(std::string name,Component* component) {
			MemoryManager::Instance().RegisterComponent(name, component);
			mComponentName = name;
		}

		HOLLOW_API virtual void Init() = 0;
		HOLLOW_API virtual std::type_index GetIndex() = 0;
		HOLLOW_API virtual void Serialize(rapidjson::Value::Object data) = 0;
		HOLLOW_API virtual void Clear() = 0;
		HOLLOW_API virtual ~Component() {}
		HOLLOW_API virtual void DebugDisplay() = 0;
		HOLLOW_API virtual void DeSerialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) { }

		std::string mComponentName;

		GameObject* mpOwner;
	protected:
		virtual Component* CreateComponent() = 0;
	};
}