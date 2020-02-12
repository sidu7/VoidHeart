#include <Hollow.h>
#include "MagicSystem.h"

#include "Components/Attack.h"
#include "Components/Magic.h"
#include "Components/Spell.h"

#include "Events/CycleSpellEvent.h"

#include "Hollow/Managers/EventManager.h"
#include "Hollow/Managers/GameObjectManager.h"
#include "Hollow/Managers/FrameRateController.h"

#include "GameMetaData/GameEventType.h"
#include "GameMetaData/GameObjectType.h"

namespace BulletHell
{
	MagicSystem MagicSystem::instance;

	void MagicSystem::Init()
	{
		// Set callback functions
		Hollow::EventManager::Instance().SubscribeEvent((int)GameEventType::ON_SPELL_COLLECT, EVENT_CALLBACK(MagicSystem::OnSpellCollect));
	}

	void MagicSystem::Update()
	{
		mDeltaTime = Hollow::FrameRateController::Instance().GetFrameTime();

		// Update all objects with magic component
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			// Get attack and magic component pointers
			Attack* pAttack = mGameObjects[i]->GetComponent<Attack>();
			Magic* pMagic = mGameObjects[i]->GetComponent<Magic>();

			// Check if player wants to cycle/change spells
			UpdateSelectedSpells(pMagic);

			// Update spell cooldowns
			UpdateSpellCooldowns(pMagic);

			// Check if left or right hand should fire
			if (!mWaitForInput)
			{
				mLeftHandPressed = Hollow::InputManager::Instance().IsKeyPressed("L") ||
					Hollow::InputManager::Instance().IsControllerTriggerPressed(SDL_CONTROLLER_AXIS_TRIGGERLEFT);
				mRightHandPressed = Hollow::InputManager::Instance().IsKeyPressed("R") ||
					Hollow::InputManager::Instance().IsControllerTriggerPressed(SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
			}

			// If either trigger is pressed start frame delay
			if (mLeftHandPressed || mRightHandPressed || mWaitForInput)
			{
				// Wait a few frames to check if both triggers are pressed
				mWaitForInput = true;
				++mNumberOfFrames;
			}

			// Set attack scripts based on player input over a few frames
			if (mNumberOfFrames > mFrameDelay)
			{
				// Update attack script based on button pressed
				if (mLeftHandPressed)
				{
					pAttack->mScriptPath = pMagic->mLeftHandScriptPath;
				}
				if (mRightHandPressed)
				{
					pAttack->mScriptPath = pMagic->mRightHandScriptPath;
				}
				if (mLeftHandPressed && mRightHandPressed)
				{
					pAttack->mScriptPath = pMagic->mCombineHandScriptPath;
				}

				// Set attack flag
				// Check combined spell first
				if (mLeftHandPressed && mRightHandPressed && pMagic->mCombinedSpell != nullptr)
				{
					bool shouldAttack = (pMagic->mCombinedSpell->mCombinedCooldown < 0.016f);
					pAttack->mShouldAttack = shouldAttack;
					if (shouldAttack)
					{
						pMagic->mCombinedSpell->mCombinedCooldown = pMagic->mCombinedSpell->mCooldown * pMagic->mCombinedSpell->mCooldownModifier;
					}
				}
				else if (mLeftHandPressed && pMagic->mLeftHandSpell != nullptr)
				{
					bool shouldAttack = (pMagic->mLeftHandSpell->mLeftHandCooldown < 0.016f);
					pAttack->mShouldAttack = shouldAttack;
					if (shouldAttack)
					{
						pMagic->mLeftHandSpell->mLeftHandCooldown = pMagic->mLeftHandSpell->mCooldown * pMagic->mLeftHandSpell->mCooldownModifier;
					}
				}
				else if (mRightHandPressed && pMagic->mRightHandSpell != nullptr)
				{
					bool shouldAttack = (pMagic->mRightHandSpell->mRightHandCooldown < 0.016f);
					pAttack->mShouldAttack = shouldAttack;
					if (shouldAttack)
					{
						pMagic->mRightHandSpell->mRightHandCooldown = pMagic->mRightHandSpell->mCooldown * pMagic->mRightHandSpell->mCooldownModifier;
					}
				}

				// Reset input tracking
				mRightHandPressed = false;
				mLeftHandPressed = false;
				mWaitForInput = false;
				mNumberOfFrames = 0;
			}
		}
	}

	void MagicSystem::AddGameObject(Hollow::GameObject* pGameObject)
	{
		CheckAllComponents<Magic, Attack>(pGameObject);
	}

	void MagicSystem::HandleBroadcastEvent(Hollow::GameEvent& event)
	{
	}

	void MagicSystem::UpdateSelectedSpells(Magic* pMagic)
	{
		// Check if either hand spell should cycle
		//bool leftHandCycle = Hollow::InputManager::Instance().IsKeyTriggered("1") ||

		bool leftHandCycle = Hollow::InputManager::Instance().IsControllerButtonPressed(SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
		bool rightHandCycle = Hollow::InputManager::Instance().IsControllerButtonPressed(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);

		// Update script paths
		if (leftHandCycle)
		{
			if (Hollow::InputManager::Instance().IsControllerButtonPressed(SDL_CONTROLLER_BUTTON_B))
			{
				if (Magic::mBasicSpells.at(FIRE)->mCollected)
				{
					pMagic->mLeftHandSpell = Magic::mBasicSpells.at(FIRE);
					// Fire spell cycle event
					CycleSpellEvent cycleEvent("left");
					Hollow::EventManager::Instance().BroadcastToSubscribers(cycleEvent);
				}
			}
			if (Hollow::InputManager::Instance().IsControllerButtonPressed(SDL_CONTROLLER_BUTTON_Y))
			{
				if (Magic::mBasicSpells.at(AIR)->mCollected)
				{
					pMagic->mLeftHandSpell = Magic::mBasicSpells.at(AIR);
					// Fire spell cycle event
					CycleSpellEvent cycleEvent("left");
					Hollow::EventManager::Instance().BroadcastToSubscribers(cycleEvent);
				}
			}
			if (Hollow::InputManager::Instance().IsControllerButtonPressed(SDL_CONTROLLER_BUTTON_A))
			{
				if (Magic::mBasicSpells.at(EARTH)->mCollected)
				{
					pMagic->mLeftHandSpell = Magic::mBasicSpells.at(EARTH);
					// Fire spell cycle event
					CycleSpellEvent cycleEvent("left");
					Hollow::EventManager::Instance().BroadcastToSubscribers(cycleEvent);
				}
			}
			if (Hollow::InputManager::Instance().IsControllerButtonPressed(SDL_CONTROLLER_BUTTON_X))
			{
				if (Magic::mBasicSpells.at(WATER)->mCollected)
				{
					pMagic->mLeftHandSpell = Magic::mBasicSpells.at(WATER);
					// Fire spell cycle event
					CycleSpellEvent cycleEvent("left");
					Hollow::EventManager::Instance().BroadcastToSubscribers(cycleEvent);
				}
			}
		}
		if (rightHandCycle)
		{
			if (Hollow::InputManager::Instance().IsControllerButtonPressed(SDL_CONTROLLER_BUTTON_B))
			{
				if (Magic::mBasicSpells.at(FIRE)->mCollected)
				{
					pMagic->mRightHandSpell = Magic::mBasicSpells.at(FIRE);
					// Fire spell cycle event
					CycleSpellEvent cycleEvent("right");
					Hollow::EventManager::Instance().BroadcastToSubscribers(cycleEvent);
				}
			}
			if (Hollow::InputManager::Instance().IsControllerButtonPressed(SDL_CONTROLLER_BUTTON_Y))
			{
				if (Magic::mBasicSpells.at(AIR)->mCollected)
				{
					pMagic->mRightHandSpell = Magic::mBasicSpells.at(AIR);
					// Fire spell cycle event
					CycleSpellEvent cycleEvent("right");
					Hollow::EventManager::Instance().BroadcastToSubscribers(cycleEvent);
				}
			}
			if (Hollow::InputManager::Instance().IsControllerButtonPressed(SDL_CONTROLLER_BUTTON_A))
			{
				if (Magic::mBasicSpells.at(EARTH)->mCollected)
				{
					pMagic->mRightHandSpell = Magic::mBasicSpells.at(EARTH);
					// Fire spell cycle event
					CycleSpellEvent cycleEvent("right");
					Hollow::EventManager::Instance().BroadcastToSubscribers(cycleEvent);
				}
			}
			if (Hollow::InputManager::Instance().IsControllerButtonPressed(SDL_CONTROLLER_BUTTON_X))
			{
				if (Magic::mBasicSpells.at(WATER)->mCollected)
				{
					pMagic->mRightHandSpell = Magic::mBasicSpells.at(WATER);
					// Fire spell cycle event
					CycleSpellEvent cycleEvent("right");
					Hollow::EventManager::Instance().BroadcastToSubscribers(cycleEvent);
				}
			}
		}

		if (pMagic->mLeftHandSpell != nullptr)
		{
			pMagic->mLeftHandScriptPath = pMagic->mLeftHandSpell->mScriptPath;
		}
		if (pMagic->mRightHandSpell != nullptr)
		{
			pMagic->mRightHandScriptPath = pMagic->mRightHandSpell->mScriptPath;
		}

		// Update combined spell script
		if (pMagic->mLeftHandSpell == nullptr || pMagic->mRightHandSpell == nullptr)
		{
			return;
		}
		int combinedSpell = pMagic->mLeftHandSpell->mSpellType | pMagic->mRightHandSpell->mSpellType;
		pMagic->mCombinedSpell = pMagic->mCombinedSpells.at(combinedSpell);
		pMagic->mCombineHandScriptPath = pMagic->mCombinedSpell->mScriptPath;
	}

	void MagicSystem::UpdateSpellCooldowns(Magic* pMagic)
	{
		// Updated all spell cooldowns for left AND right hand
		for (auto& spell : pMagic->mBasicSpells)
		{
			spell.second->mLeftHandCooldown = std::max(0.0f, spell.second->mLeftHandCooldown - mDeltaTime);
			spell.second->mRightHandCooldown = std::max(0.0f, spell.second->mRightHandCooldown - mDeltaTime);
		}

		// Update all combined spell cooldowns
		for (auto& spell : pMagic->mCombinedSpells)
		{
			spell.second->mCombinedCooldown = std::max(0.0f, spell.second->mCombinedCooldown - mDeltaTime);
		}
	}

	void MagicSystem::OnSpellCollect(Hollow::GameEvent& event)
	{
		// Add the spell to the list of spells
		Hollow::GameObject* pSpellObject = event.mpObject1->mType == (int)GameObjectType::SPELL ? event.mpObject1 : event.mpObject2;
		Hollow::GameObject* pPlayer = event.mpObject1->mType == (int)GameObjectType::PLAYER ? event.mpObject1 : event.mpObject2;

		// Get magic component from player to set spell active flag
		Magic* pPlayerMagic = pPlayer->GetComponent<Magic>();
		pPlayerMagic->mBasicSpells[pSpellObject->GetComponent<Spell>()->mSpellType]->mCollected = true;

		// Check if player has initial spell values set
		if (pPlayerMagic->mLeftHandSpell == nullptr)
		{
			pPlayerMagic->mLeftHandSpell = pPlayerMagic->mBasicSpells[pSpellObject->GetComponent<Spell>()->mSpellType];
			// Fire spell cycle event
			CycleSpellEvent cycleEvent("left");
			Hollow::EventManager::Instance().BroadcastToSubscribers(cycleEvent);
		}
		if (pPlayerMagic->mRightHandSpell == nullptr)
		{
			pPlayerMagic->mRightHandSpell = pPlayerMagic->mBasicSpells[pSpellObject->GetComponent<Spell>()->mSpellType];
			// Fire spell cycle event
			CycleSpellEvent cycleEvent("right");
			Hollow::EventManager::Instance().BroadcastToSubscribers(cycleEvent);
		}

		// Destroy spell object
		Hollow::GameObjectManager::Instance().DeleteGameObject(pSpellObject);
	}
}