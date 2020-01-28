#include <Hollow.h>
#include "MagicSystem.h"

#include "Components/Attack.h"
#include "Components/Magic.h"
#include "Components/Spell.h"

#include "Hollow/Managers/InputManager.h"
#include "Hollow/Managers/EventManager.h"
#include "Hollow/Managers/GameObjectManager.h"

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
		// Update all objects with magic component
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			// Get attack and magic component pointers
			Attack* pAttack = mGameObjects[i]->GetComponent<Attack>();
			Magic* pMagic = mGameObjects[i]->GetComponent<Magic>();

			// Check if player wants to cycle/change spells
			UpdateSelectedSpells(pMagic);

			// Check if left or right hand should fire
			bool leftHandPressed = Hollow::InputManager::Instance().IsKeyPressed("L") ||
				Hollow::InputManager::Instance().IsControllerTriggerTriggered(SDL_CONTROLLER_AXIS_TRIGGERLEFT);
			bool rightHandPressed = Hollow::InputManager::Instance().IsKeyPressed("R") ||
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

	void MagicSystem::HandleBroadcastEvent(Hollow::GameEvent& event)
	{
	}

	void MagicSystem::UpdateSelectedSpells(Magic* pMagic)
	{
		// Check if either hand spell should cycle
		bool leftHandCycle = Hollow::InputManager::Instance().IsKeyTriggered("1") ||
			Hollow::InputManager::Instance().IsControllerButtonTriggered(SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
		bool rightHandCycle = Hollow::InputManager::Instance().IsKeyPressed("2") ||
			Hollow::InputManager::Instance().IsControllerButtonPressed(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);

		// Update script paths
		if (leftHandCycle)
		{
			// Get next spell
			pMagic->mLeftHandSpell = GetNextSpell(pMagic, pMagic->mLeftHandSpell);
			pMagic->mLeftHandScriptPath = pMagic->mLeftHandSpell->mScriptPath;
		}
		if (rightHandCycle)
		{
			// Get next spell
			pMagic->mRightHandSpell = GetNextSpell(pMagic, pMagic->mLeftHandSpell);
			pMagic->mRightHandScriptPath = pMagic->mLeftHandSpell->mScriptPath;
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
		Magic::SpellData* pSpellToAdd = new Magic::SpellData{ pSpell->mName, pSpell->mScriptPath };
		pPlayerMagic->mSpells.push_back(pSpellToAdd);

		// Destroy spell object
		Hollow::GameObjectManager::Instance().DeleteGameObject(pSpellObject);
	}
}