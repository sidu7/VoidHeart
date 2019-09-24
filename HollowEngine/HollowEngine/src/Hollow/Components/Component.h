#pragma once
#include "Hollow/Managers/MemoryManager.h"

namespace Hollow
{
	class HOLLOW_API Component
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

		virtual void Init() = 0;
		virtual std::type_index GetIndex() = 0;
		virtual void Serialize(rapidjson::Value::Object data) = 0;
		virtual void Clear() = 0;
		virtual ~Component() {}
		virtual void DebugDisplay() = 0;

		std::string mComponentName;

	protected:
		virtual Component* CreateComponent() = 0;
	};
}