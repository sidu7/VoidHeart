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
			
			lua["gameObject"] = scriptComp->mpOwner;

			for (auto script : scriptComp->mScripts)
			{
				lua.script_file(ScriptingManager::Instance().rootPath + script + ScriptingManager::Instance().ext);
			}
		}

		// Deprecated
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			Script* script = mGameObjects[i]->GetComponent<Script>();

			Body* pBody = mGameObjects[i]->GetComponent<Body>();
			Camera* pCam = mGameObjects[i]->GetComponent<Camera>();

			// setting body rotation to match camera rotation about y
			/*pBody->mQuaternion = glm::toQuat(glm::rotate(glm::mat4(1.0f),
				glm::radians(-pCam->mYaw + 90), glm::vec3(0.0f, 1.0f, 0.0f)));*/

			auto& lua = ScriptingManager::Instance().lua;

			lua["fpsCamera"] = pCam;
			lua["player"] = pBody;

			//lua.script_file(script->mScriptPath);
			if (lua["jump"])
			{
				glm::vec3 jump = lua.get<glm::vec3>("impulse");
				jump.x *= 2.0f;
				jump.z *= 2.0f;
				PhysicsManager::Instance().ApplyLinearImpulse(mGameObjects[i],jump );
			}
			
			PhysicsManager::Instance().ApplyAngularImpulse(mGameObjects[i], lua.get<glm::vec3>("impulse"));
		}
	}

	void ScriptingSystem::AddGameObject(GameObject* pGameObject)
	{
		CheckAllComponents<Script>(pGameObject);
	}
}
