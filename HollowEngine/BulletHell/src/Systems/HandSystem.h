#pragma once
#include "Hollow/Systems/System.h"

namespace BulletHell
{
	class HandSystem : public Hollow::System
	{
		REGISTERSYSTEM(HandSystem, 4);
	public:
		void Init() override;
		void OnSceneInit() override;
		void Update() override;
		void AddGameObject(Hollow::GameObject* pGameObject) override;

        void OnDeleteAllGameObjects() override;
		void HandleBroadcastEvent(Hollow::GameEvent& event) override;
        static Hollow::GameObject* CreateUIObject(const glm::vec2& scale, const glm::vec2& position, const int layer, const std::string& texturePath, const std::string& objectFilePath = "Resources/Prefabs/UIIcon.json");
	    
	private:
		Hollow::GameObject* CreateHand(const glm::vec3& offset, const std::string& tag, const glm::vec2& UIScale, const glm::vec2& UIPosition);
		void CreateUIObjects();
		void SubscribeToEvents();

		void UpdateSpellUI();
		void UpdateHandSpellUI(const std::string& handString);
		void UpdateCombinedSpellUI();

		void CycleSpell(Hollow::GameEvent& event);
		void CollectSpell(Hollow::GameEvent& event);

	private:
		Hollow::GameObject* mpLeftHandUI;
		Hollow::GameObject* mpRightHandUI;
		Hollow::GameObject* mpCombinedHandUI;
		Hollow::GameObject* mpCombinedCooldownUI;

		Hollow::GameObject* mpLeftHand;
		Hollow::GameObject* mpRightHand;

		Hollow::GameObject* mpPlayerObject;

		typedef std::array<Hollow::GameObject*, 4> SpellUIArray;
		SpellUIArray mLeftHandUI;
		SpellUIArray mRightHandUI;

		const glm::vec3 mUncollectedColor = glm::vec3(-0.7, -0.7, -0.7);
	};
}