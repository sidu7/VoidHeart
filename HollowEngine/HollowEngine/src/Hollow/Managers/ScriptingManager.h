#pragma once
#include "Hollow/Common.h"

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace Hollow {
	class ScriptingManager {
		SINGLETON(ScriptingManager)
	public:
		HOLLOW_API  void Init();

		sol::state lua;
	};
}
