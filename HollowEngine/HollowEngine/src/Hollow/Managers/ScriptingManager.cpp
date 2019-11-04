#include <hollowpch.h>
#include "ScriptingManager.h"

namespace Hollow
{
	void ScriptingManager::Init()
	{
		lua.open_libraries(sol::lib::base);

		lua.new_usertype<glm::vec3>("vec3",
			sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(),
			"x", &glm::vec3::x,
			"y", &glm::vec3::y,
			"z", &glm::vec3::z
			);
	}

}