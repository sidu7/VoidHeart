#include "MovementSystem.h"

#include "BulletHell/Components/Movement.h"

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

			lua["player"] = pBody;

			lua["isMoveForward"] = (Hollow::InputManager::Instance().IsKeyPressed(SDL_SCANCODE_UP) || Hollow::InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_LEFTY) < -16000);

			lua["isMoveBackward"] = (Hollow::InputManager::Instance().IsKeyPressed(SDL_SCANCODE_DOWN) || Hollow::InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_LEFTY) > 16000);

			lua["isStrafeLeft"] = (Hollow::InputManager::Instance().IsKeyPressed(SDL_SCANCODE_LEFT) || Hollow::InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_LEFTX) < -16000);

			lua["isStrafeRight"] = (Hollow::InputManager::Instance().IsKeyPressed(SDL_SCANCODE_RIGHT) || Hollow::InputManager::Instance().GetAxisValue(SDL_CONTROLLER_AXIS_LEFTX) > 16000);

			lua["jump"] = (Hollow::InputManager::Instance().IsPressed(SDL_CONTROLLER_BUTTON_B));

			lua["dash"] = (Hollow::InputManager::Instance().IsPressed(SDL_CONTROLLER_BUTTON_X));

			lua.script_file(script->mScriptPath);
			glm::vec3 jump = lua.get<glm::vec3>("impulse");
			Hollow::PhysicsManager::Instance().ApplyLinearImpulse(mGameObjects[i], jump);
		}
	}

	void MovementSystem::AddGameObject(Hollow::GameObject* pGameObject)
	{
		CheckAllComponents<Movement, Hollow::Body>(pGameObject);
	}
}