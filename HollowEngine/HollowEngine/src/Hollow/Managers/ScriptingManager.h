#pragma once
#include "Hollow/Common.h"

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace Hollow {
	/// <summary>
	/// Class Scripting Manager.
	/// </summary>
	class ScriptingManager {
		SINGLETON(ScriptingManager)
	public:
		/// <summary>
		/// Initializes manager data.
		/// </summary>
		HOLLOW_API  void Init();

	public:
		sol::state lua;
	};
}
