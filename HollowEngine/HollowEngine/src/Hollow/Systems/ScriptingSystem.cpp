#include <hollowpch.h>
#include "ScriptingSystem.h"
#include "Hollow/Components/Script.h"
#include "Hollow/Managers/ScriptingManager.h"
#include "Hollow/Components/Transform.h"
#include "Hollow/Components/Camera.h"
#include "Hollow/Managers/InputManager.h"
#include "Hollow/Components/Body.h"
#include "Hollow/Managers/PhysicsManager.h"



namespace Hollow
{
	ScriptingSystem ScriptingSystem::instance;

	void ScriptingSystem::Update()
	{
		auto& lua = ScriptingManager::Instance().lua;
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			Script* scriptComp = mGameObjects[i]->GetComponent<Script>();

			if (scriptComp->mIsActive)
			{
				lua["gameObject"] = scriptComp->mpOwner;

				for (auto script : scriptComp->mScripts)
				{
					try {
						lua.script_file(ScriptingManager::Instance().rootPath + script + ScriptingManager::Instance().ext);
					}
					catch (const sol::error & e)
					{
						const char* errorName = e.what();
						HW_TRACE(errorName);
					}
				}
			}
		}
	}

	void ScriptingSystem::AddGameObject(GameObject* pGameObject)
	{
		CheckAllComponents<Script>(pGameObject);
	}

	void ScriptingSystem::OnDeleteGameObject(GameObject* pGameObject)
	{
		// Run any delete scripts
		auto& lua = ScriptingManager::Instance().lua;
		Script* pScript = pGameObject->GetComponent<Script>();
		lua["gameObject"] = pScript->mpOwner;
		for (auto& script : pScript->mDestroyScripts)
		{
			lua.script_file(ScriptingManager::Instance().rootPath + script + ScriptingManager::Instance().ext);
		}
	}
}
