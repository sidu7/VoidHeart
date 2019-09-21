#pragma once
#include "../Managers/MemoryManager.h"

#define REGISTER(x) public:													\
					std::type_index GetIndex()								\
					{														\
						return std::type_index(typeid(x));		\
					}														\
					private:												\
					static x instance;										\
					x* CreateComponent()									\
					{														\
						return new x();										\
					}


namespace Hollow
{
	class HOLLOW_API Component
	{
		friend class MemoryManager;
	public:
		Component(const char* name,Component* component) {
			MemoryManager::Instance().RegisterComponent(name, component);
			mComponentName = name;
		}

		virtual std::type_index GetIndex() = 0;
		virtual void Clear() = 0;
		virtual ~Component() {}
		
		std::string mComponentName;
	private:
		virtual Component* CreateComponent() = 0;
	};
}