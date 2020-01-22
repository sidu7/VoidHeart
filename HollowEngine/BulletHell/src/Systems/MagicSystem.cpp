#include <Hollow.h>
#include "MagicSystem.h"

#include "Components/Attack.h"
#include "Components/Magic.h"

#include "Hollow/Managers/InputManager.h"

namespace BulletHell
{
	MagicSystem MagicSystem::instance;

	void MagicSystem::Update()
	{
		// Update all objects with magic component
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			// Get attack and magic component pointers
			Attack* pAttack = mGameObjects[i]->GetComponent<Attack>();
			Magic* pMagic = mGameObjects[i]->GetComponent<Magic>();

			// Check if left or right hand should fire
			bool leftHandPressed = Hollow::InputManager::Instance().IsKeyPressed(SDL_SCANCODE_L) ||
				Hollow::InputManager::Instance().IsControllerTriggerTriggered(SDL_CONTROLLER_AXIS_TRIGGERLEFT);
			bool rightHandPressed = Hollow::InputManager::Instance().IsKeyPressed(SDL_SCANCODE_R) ||
				Hollow::InputManager::Instance().IsControllerTriggerTriggered(SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

			// Update attack script based on button pressed
			if (leftHandPressed)
			{
				pAttack->mScriptPath = pMagic->mLeftHandScriptPath;
			}
			if (rightHandPressed)
			{
				pAttack->mScriptPath = pMagic->mRightHandScriptPath;
			}
			if (leftHandPressed && rightHandPressed)
			{
				pAttack->mScriptPath = pMagic->mCombineHandScriptPath;
			}
		}
	}

	void MagicSystem::AddGameObject(Hollow::GameObject* pGameObject)
	{
		CheckAllComponents<Magic, Attack>(pGameObject);
	}
}