#pragma once
#include "Hollow/Systems/System.h"

#include "Hollow/Managers/InputManager.h"

#include "Components/Magic.h"
#include "Components/Spell.h"

namespace BulletHell
{

	class MagicSystem : public Hollow::System
	{
		// Set priority to be before attack system
		REGISTERSYSTEM(MagicSystem, 2);
	public:
		void Init() override;
		void Update() override;
		void AddGameObject(Hollow::GameObject* pGameObject) override;
		void HandleBroadcastEvent(Hollow::GameEvent& event) override;

	private:
		void UpdateSelectedSpells(Magic* pMagic);
		void UpdateSpellCooldowns(Magic* pMagic);

		bool CheckSpellInput(SpellType spellType);
		bool CheckSpellCollected(SpellType spellType);

		void CycleHandToSpell(Magic::SpellData** hand, SpellType spellType, const std::string& handString);

		void CheckAndCycleSpell(Magic::SpellData** hand, SpellType spellType, const std::string& handString);

		// Event handling
		void OnSpellCollect(Hollow::GameEvent& event);
		void OnPlayerDeath(Hollow::GameEvent& event);

	private:
		float mDeltaTime;
		int mNumberOfFrames = 0;
		const int mFrameDelay = 5;
		bool mWaitForInput = false;
		bool mLeftHandPressed = false;
		bool mRightHandPressed = false;
		const std::unordered_map< SpellType, SDL_GameControllerButton> mSpellButtonMap = {
								{FIRE, SDL_CONTROLLER_BUTTON_B},
								{AIR, SDL_CONTROLLER_BUTTON_Y},
								{EARTH, SDL_CONTROLLER_BUTTON_A},
								{WATER, SDL_CONTROLLER_BUTTON_X} };
	};
}