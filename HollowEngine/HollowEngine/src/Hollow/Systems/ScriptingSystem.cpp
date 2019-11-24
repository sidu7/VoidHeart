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
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			Script* script = mGameObjects[i]->GetComponent<Script>();
			Body* pBody = mGameObjects[i]->GetComponent<Body>();
			Camera* pCam = mGameObjects[i]->GetComponent<Camera>();

			// setting body rotation to match camera rotation about y
			pBody->mQuaternion = glm::toQuat(glm::rotate(glm::mat4(1.0f),
				glm::radians(-pCam->mYaw + 90), glm::vec3(0.0f, 1.0f, 0.0f)));

			auto& lua = ScriptingManager::Instance().lua;

			lua["fpsCamera"] = pCam;
			lua["player"] = pBody;

			lua["isMoveForward"] = (InputManager::Instance().IsKeyPressed(SDL_SCANCODE_UP) || InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_LEFTY) < -16000);

			lua["isMoveBackward"] = (InputManager::Instance().IsKeyPressed(SDL_SCANCODE_DOWN) || InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_LEFTY) > 16000);

			lua["isStrafeLeft"] = (InputManager::Instance().IsKeyPressed(SDL_SCANCODE_LEFT) || InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_LEFTX) < -16000);

			lua["isStrafeRight"] = (InputManager::Instance().IsKeyPressed(SDL_SCANCODE_RIGHT) || InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_LEFTX) > 16000);

			lua["jump"] = (InputManager::Instance().IsPressed(SDL_CONTROLLER_BUTTON_B));

			if (InputManager::Instance().IsKeyTriggered(SDL_SCANCODE_C) || InputManager::Instance().IsPressed((SDL_CONTROLLER_BUTTON_A)))
			{
				pCam->mIsActive = !pCam->mIsActive;
				if (pCam->mIsActive)
				{
					InputManager::Instance().HideMouseCursor();
				}
				else
				{
					InputManager::Instance().ShowMouseCursor();
				}
			}

			lua.script_file(script->mScriptPath);

			PhysicsManager::Instance().ApplyImpulse(mGameObjects[i], lua.get<glm::vec3>("impulse"));

		}
	}

	void ScriptingSystem::AddGameObject(GameObject* pGameObject)
	{
		CheckAllComponents<Script>(pGameObject);
	}
}
