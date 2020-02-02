#pragma once
#include "Hollow/Systems/System.h"

#include "Components/Magic.h"

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

		Magic::SpellData* GetNextSpell(Magic* pMagic, Magic::SpellData* pSpellData);

		void OnSpellCollect(Hollow::GameEvent& event);

	private:
		float mDeltaTime;
		int mNumberOfFrames = 0;
		const int mFrameDelay = 5;
		bool mWaitForInput = false;
		bool mLeftHandPressed = false;
		bool mRightHandPressed = false;
	};
}