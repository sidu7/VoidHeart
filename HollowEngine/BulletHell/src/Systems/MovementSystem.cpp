#include <Hollow.h>
#include "MovementSystem.h"

#include "Components/Movement.h"

#include "Hollow/Components/Body.h"

#include "Hollow/Managers/ScriptingManager.h"
#include "Hollow/Managers/InputManager.h"
#include "Hollow/Managers/PhysicsManager.h"

namespace BulletHell
{
	MovementSystem MovementSystem::instance;

	void MovementSystem::Update()
	{
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			Movement* script = mGameObjects[i]->GetComponent<Movement>();
			Hollow::Body* pBody = mGameObjects[i]->GetComponent<Hollow::Body>();

			auto& lua = Hollow::ScriptingManager::Instance().lua;

			lua["body"] = pBody;

			lua["isMoveForward"] = (Hollow::InputManager::Instance().IsKeyPressed(SDL_SCANCODE_UP) || Hollow::InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_LEFTY) < -16000);

			lua["isMoveBackward"] = (Hollow::InputManager::Instance().IsKeyPressed(SDL_SCANCODE_DOWN) || Hollow::InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_LEFTY) > 16000);

			lua["isStrafeLeft"] = (Hollow::InputManager::Instance().IsKeyPressed(SDL_SCANCODE_LEFT) || Hollow::InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_LEFTX) < -16000);

			lua["isStrafeRight"] = (Hollow::InputManager::Instance().IsKeyPressed(SDL_SCANCODE_RIGHT) || Hollow::InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_LEFTX) > 16000);

			//lua["dash"] = (Hollow::InputManager::Instance().IsControllerButtonTriggered(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER));
			//lua["dash"] = (Hollow::InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_TRIGGERLEFT) > 5000);
			lua["dash"] = (Hollow::InputManager::Instance().IsControllerTriggerTriggered(SDL_CONTROLLER_AXIS_TRIGGERLEFT));

			lua.script_file(script->mScriptPath);
			glm::vec3 impulse = lua.get<glm::vec3>("impulse");
			Hollow::PhysicsManager::Instance().ApplyLinearImpulse(mGameObjects[i], impulse);
		}
	}

	void MovementSystem::AddGameObject(Hollow::GameObject* pGameObject)
	{
		CheckAllComponents<Movement, Hollow::Body>(pGameObject);
	}
}