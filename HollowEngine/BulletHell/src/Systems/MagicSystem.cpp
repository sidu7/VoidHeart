#include <Hollow.h>
#include "MagicSystem.h"

#include "Components/Attack.h"
#include "Components/Magic.h"
#include "Components/Spell.h"

#include "Events/CycleSpellEvent.h"

#include "Hollow/Managers/InputManager.h"
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

			if (pMagic->mLeftHandSpell == nullptr || pMagic->mRightHandSpell == nullptr)
			{
				continue;
			}

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
				if (mLeftHandPressed && mRightHandPressed && (pMagic->mCombinedSpell->mCombinedCooldown < 0.016f))
				{
					pAttack->mShouldAttack = true;
					pMagic->mCombinedSpell->mCombinedCooldown = pMagic->mCombinedSpell->mCooldown * pMagic->mCombinedSpell->mCooldownModifier;
				}
				else if (mLeftHandPressed && (pMagic->mLeftHandSpell->mLeftHandCooldown < 0.016f))
				{
					pAttack->mShouldAttack = true;
					pMagic->mLeftHandSpell->mLeftHandCooldown = pMagic->mLeftHandSpell->mCooldown * pMagic->mLeftHandSpell->mCooldownModifier;
				}
				else if (mRightHandPressed && (pMagic->mRightHandSpell->mRightHandCooldown < 0.016f))
				{
					pAttack->mShouldAttack = true;
					pMagic->mRightHandSpell->mRightHandCooldown = pMagic->mRightHandSpell->mCooldown * pMagic->mRightHandSpell->mCooldownModifier;
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
		bool leftHandCycle = Hollow::InputManager::Instance().IsKeyTriggered("1") ||
			Hollow::InputManager::Instance().IsControllerButtonTriggered(SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
		bool rightHandCycle = Hollow::InputManager::Instance().IsKeyTriggered("2") ||
			Hollow::InputManager::Instance().IsControllerButtonTriggered(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);

		// Update script paths
		if (leftHandCycle)
		{
			// Get next spell
			pMagic->mLeftHandSpell = GetNextSpell(pMagic, pMagic->mLeftHandSpell);
			pMagic->mLeftHandScriptPath = pMagic->mLeftHandSpell->mScriptPath;

			// Fire spell cycle event
			CycleSpellEvent cycleEvent("left");
			Hollow::EventManager::Instance().BroadcastToSubscribers(cycleEvent);
		}
		if (rightHandCycle)
		{
			// Get next spell
			pMagic->mRightHandSpell = GetNextSpell(pMagic, pMagic->mRightHandSpell);
			pMagic->mRightHandScriptPath = pMagic->mRightHandSpell->mScriptPath;

			// Fire spell cycle event
			CycleSpellEvent cycleEvent("right");
			Hollow::EventManager::Instance().BroadcastToSubscribers(cycleEvent);
		}

		// Update combined spell script
		int combinedSpell = pMagic->mLeftHandSpell->mSpellType | pMagic->mRightHandSpell->mSpellType;
		pMagic->mCombinedSpell = pMagic->mCombinedSpells.at(combinedSpell);
		pMagic->mCombineHandScriptPath = pMagic->mCombinedSpell->mScriptPath;
	}

	void MagicSystem::UpdateSpellCooldowns(Magic* pMagic)
	{
		// Updated all spell cooldowns for left AND right hand
		for (auto& spell : pMagic->mSpells)
		{
			spell->mLeftHandCooldown = std::max(0.0f, spell->mLeftHandCooldown - mDeltaTime);
			spell->mRightHandCooldown = std::max(0.0f, spell->mRightHandCooldown - mDeltaTime);
		}

		// Update all combined spell cooldowns
		for (auto& spell : pMagic->mCombinedSpells)
		{
			spell.second->mCombinedCooldown = std::max(0.0f, spell.second->mCombinedCooldown - mDeltaTime);
		}
	}

	Magic::SpellData* MagicSystem::GetNextSpell(Magic* pMagic, Magic::SpellData* pSpellData)
	{
		// TODO: Find a better way to do this, maybe make circular doubly linked list
		auto& spellIterator = std::find(pMagic->mSpells.begin(), pMagic->mSpells.end(), pSpellData);
		auto& nextSpell = std::next(spellIterator, 1);
		return (nextSpell != pMagic->mSpells.end()) ? *nextSpell : *pMagic->mSpells.begin();
	}

	void MagicSystem::OnSpellCollect(Hollow::GameEvent& event)
	{
		// Add the spell to the list of spells
		Hollow::GameObject* pSpellObject = event.mpObject1->mType == (int)GameObjectType::SPELL ? event.mpObject1 : event.mpObject2;
		Hollow::GameObject* pPlayer = event.mpObject1->mType == (int)GameObjectType::PLAYER ? event.mpObject1 : event.mpObject2;

		// Get magic component from player to add spell data to
		Magic* pPlayerMagic = pPlayer->GetComponent<Magic>();

		// Create new spell to add to player list
		Spell* pSpell = pSpellObject->GetComponent<Spell>();
		Magic::SpellData* pSpellToAdd = new Magic::SpellData{ pSpell->mName, pSpell->mScriptPath, pSpell->mSpellType,  pSpell->mUIRotation, pSpell->mParticleSize, pSpell->mParticleTexturePath, pSpell->mCooldown };
		pPlayerMagic->mSpells.push_back(pSpellToAdd);

		// Check if player has initial spell values set
		if (pPlayerMagic->mLeftHandSpell == nullptr)
		{
			pPlayerMagic->mLeftHandSpell = pPlayerMagic->mSpells[0];
		}
		if (pPlayerMagic->mRightHandSpell == nullptr)
		{
			pPlayerMagic->mRightHandSpell = pPlayerMagic->mSpells[0];
		}
		if (pPlayerMagic->mCombinedSpell == nullptr)
		{
			int combinedSpellIndex = pPlayerMagic->mRightHandSpell->mSpellType & pPlayerMagic->mLeftHandSpell->mSpellType;
			//pPlayerMagic->mCombinedSpell = mpCombinedSpellMap.at(combinedSpellIndex);
		}

		// Destroy spell object
		Hollow::GameObjectManager::Instance().DeleteGameObject(pSpellObject);
	}
}