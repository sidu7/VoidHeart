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
		HOLLOW_API  void Init(rapidjson::Value::Object& data);

		// Add function for calling a script
		HOLLOW_API void RunScript(std::string name, std::string folderPath = "");
	public:
		sol::state lua;
		sol::usertype<GameObject> mGameObjectType;
		std::string rootPath;
		std::string ext;
	};
}
