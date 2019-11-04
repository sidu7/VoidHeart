#include <hollowpch.h>
#include "ScriptingSystem.h"
#include "Hollow/Components/Script.h"
#include "Hollow/Managers/ScriptingManager.h"
#include "Hollow/Components/Transform.h"
#include "Hollow/Managers/InputManager.h"
#include "Hollow/Components/Body.h"
#include "Hollow/Managers/PhysicsManager.h"

namespace Hollow
{
	ScriptingSystem ScriptingSystem::instance;
	
	void ScriptingSystem::Update()
	{
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			Script* script = mGameObjects[i]->GetComponent<Script>();
			Body* pBody = mGameObjects[i]->GetComponent<Body>();

			auto& lua = ScriptingManager::Instance().lua;
			
			lua["isMoveForward"] = InputManager::Instance().IsKeyPressed(SDL_SCANCODE_UP);
			lua["isMoveBackward"] = InputManager::Instance().IsKeyPressed(SDL_SCANCODE_DOWN);
			lua["isStrafeLeft"] = InputManager::Instance().IsKeyPressed(SDL_SCANCODE_LEFT);
			lua["isStrafeRight"] = InputManager::Instance().IsKeyPressed(SDL_SCANCODE_RIGHT);
			
			lua.script_file(script->scriptPath);

			PhysicsManager::Instance().ApplyImpulse(mGameObjects[i], lua.get<glm::vec3>("impulse"));
			
		}
	}

	void ScriptingSystem::AddGameObject(GameObject* pGameObject)
	{
		CheckComponents<Script>(pGameObject);
	}
}
