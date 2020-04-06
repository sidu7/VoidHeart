#include <Hollow.h>
#include "HandSystem.h"

#include "Components/ParentOffset.h"
#include "Components/Magic.h"
#include "Components/Spell.h"

#include "GameLogic/GameLogicManager.h"

#include "Events/CycleSpellEvent.h"

#include "Hollow/Components/Transform.h"
#include "Hollow/Components/UIImage.h"
#include "Hollow/Components/UITransform.h"
#include "Hollow/Components/ParticleEmitter.h"
#include "Hollow/Components/Body.h"
#include "Hollow/Components/Light.h"

#include "Hollow/Managers/ResourceManager.h"
#include "Hollow/Managers/EventManager.h"
#include "Hollow/Managers/InputManager.h"
#include "Hollow/Managers/GameObjectManager.h"
#include "Hollow/Managers/ScriptingManager.h"

#include "Hollow/Events/GameEvent.h"

#include "GameMetaData/GameEventType.h"
#include "GameMetaData/GameObjectType.h"

namespace BulletHell
{
	HandSystem HandSystem::instance;

	void HandSystem::Init()
	{
		// Subscribe to hand related events
		SubscribeToEvents();

		// Add the parent offset component to lua
		auto& lua = Hollow::ScriptingManager::Instance().lua;

		lua.new_usertype<ParentOffset>("ParentOffset",
			sol::constructors<ParentOffset()>(),
			"offset", &ParentOffset::mOffset
			);

		Hollow::ScriptingManager::Instance().mGameObjectType["GetParentOffset"] = &Hollow::GameObject::GetComponent<ParentOffset>;

		mGlobalObjectsInit = false;
	}

	void HandSystem::OnSceneInit()
	{
		if (!mGlobalObjectsInit)
		{
			// Create hand objects and UI, add them to global list
			InitGlobalHandObjects();
			// Hacky fix for now, can't ensure hand system will be init after player has been created
			mGlobalObjectsInit = true;
		}
	}

	void HandSystem::Update()
	{
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			Hollow::GameObject* pGameObject = mGameObjects[i];

			// Update position based on offset and parent
			Hollow::Transform* pTr = mGameObjects[i]->GetComponent<Hollow::Transform>();
			ParentOffset* pParentOffset = mGameObjects[i]->GetComponent<ParentOffset>();
			if (pParentOffset->mpParent && pParentOffset->mpParent->mActive)
			{
				Hollow::Transform* pParentTr = pParentOffset->mpParent->GetComponent<Hollow::Transform>();

				// Think about doing default forward
				if (pParentOffset->mUseForward)
				{
					// Get parent forward
					// THIS IS ONLY FOR MELEE COLLIDER FOR NOW
					// IT ARBITRARILY TAKES A SCALE AMOUNT OFF THE FORWARD VECTOR
					// USE THIS SYSTEM WITH CAUTION, IT IS A MESS
					pTr->mPosition = pParentOffset->mOffset + pParentTr->mPosition + (pParentTr->GetForward() * 0.5f);
				}
				else
				{
					// THIS ONLY WORKS UNDER THE ASSUMPTION THAT THE OBJECT
					// IS HANDS ROTATING AROUND A PLAYER
					// YOU MAY GET WACKY RESULTS IF USING THIS OUTSIDE OF PLAYER
					// IF ROTATION IS NOT DESIRED THE LINE OF CODE BELOW WILL DO SO
					//pTr->mPosition = pParentOffset->mOffset + pParentTr->mPosition;

					// Consider parent rotation
					pTr->mPosition = glm::rotateY(pParentOffset->mOffset, glm::radians(pParentTr->mRotation.y)) + pParentTr->mPosition;
				}

				Hollow::Body* pBody = mGameObjects[i]->GetComponent<Hollow::Body>();
				if (pBody)
				{
					pBody->mPosition = pTr->mPosition;
				}
			}
			else
			{
				pParentOffset->mpParent = nullptr;
				return;
			}
		}

		// Update active spell UI images
		UpdateSpellUI();

		// Update combined spell UI image
		UpdateCombinedSpellUI();

	}

	void HandSystem::AddGameObject(Hollow::GameObject* pGameObject)
	{
		CheckAllComponents<ParentOffset, Hollow::Transform>(pGameObject);
		if (pGameObject->mType == (int)GameObjectType::PLAYER)
		{
			mpPlayerObject = pGameObject;
		}
	}

	void HandSystem::OnDeleteAllGameObjects()
	{
		HW_ERROR("DELETING ALL GAME OBJECTS");
		mpCombinedHandUI = nullptr;
		mpLeftHandUI = nullptr;
		mpRightHandUI = nullptr;
		mpCombinedCooldownUI = nullptr;
		mpLeftHand = nullptr;
		mpRightHand = nullptr;
		for (auto& UI : mLeftHandUI)
		{
			UI = nullptr;
		}
		for (auto& UI : mRightHandUI)
		{
			UI = nullptr;
		}
	}

	void HandSystem::HandleBroadcastEvent(Hollow::GameEvent& event)
	{
	}

	void HandSystem::InitGlobalHandObjects()
	{
		mpLeftHand = CreateHand(glm::vec3(1.0f, 0.0f, 0.0f), "left");
		mpRightHand = CreateHand(glm::vec3(-1.0f, 0.0f, 0.0f), "right");

		CreateUIObjects();
		AddHandObjectsToGlobal();
		SetHighlightUIActive(false);
	}

	Hollow::GameObject* HandSystem::CreateHand(const glm::vec3& offset, const std::string& tag)
	{
		Hollow::GameObject* pHand = Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Prefabs/Hand.json");
		BulletHell::ParentOffset* pParentOffset = pHand->GetComponent<BulletHell::ParentOffset>();
		pParentOffset->mOffset = offset;
		pParentOffset->mTag = tag;

		return pHand;
	}

	void HandSystem::CreateUIObjects()
	{
		const int UIScale = 128;
		// Create right/left hand hightlight UI
		mpRightHandUI = CreateUIObject(glm::vec2(UIScale, UIScale), glm::vec2(1150, 128), 0, "Resources/Textures/UI/Hightlight.png");
		mpLeftHandUI = CreateUIObject(glm::vec2(UIScale, UIScale), glm::vec2(128, 128), 0, "Resources/Textures/UI/Hightlight.png");

		// Create combined hand image and cooldown UI
		mpCombinedHandUI = CreateUIObject(glm::vec2(UIScale, UIScale), glm::vec2(600, 128), 0, "Resources/Textures/UI/Combined.png");
		mpCombinedCooldownUI = CreateUIObject(glm::vec2(UIScale, UIScale), glm::vec2(600, 128), 1, "Resources/Textures/UI/CombinedCooldown.png", "Resources/Prefabs/UICooldownIcon.json");

		// Create left hand spell UI
		mLeftHandUI[0] = CreateUIObject(glm::vec2(UIScale, UIScale), glm::vec2(128, 128), 1, "Resources/Textures/UI/Fire.png");
		mLeftHandUI[1] = CreateUIObject(glm::vec2(UIScale, UIScale), glm::vec2(128, 128), 1, "Resources/Textures/UI/Air.png");
		mLeftHandUI[2] = CreateUIObject(glm::vec2(UIScale, UIScale), glm::vec2(128, 128), 1, "Resources/Textures/UI/Earth.png");
		mLeftHandUI[3] = CreateUIObject(glm::vec2(UIScale, UIScale), glm::vec2(128, 128), 1, "Resources/Textures/UI/Water.png");

		// Create right hand spell UI
		mRightHandUI[0] = CreateUIObject(glm::vec2(UIScale, UIScale), glm::vec2(1150, 128), 1, "Resources/Textures/UI/Fire.png");
		mRightHandUI[1] = CreateUIObject(glm::vec2(UIScale, UIScale), glm::vec2(1150, 128), 1, "Resources/Textures/UI/Air.png");
		mRightHandUI[2] = CreateUIObject(glm::vec2(UIScale, UIScale), glm::vec2(1150, 128), 1, "Resources/Textures/UI/Earth.png");
		mRightHandUI[3] = CreateUIObject(glm::vec2(UIScale, UIScale), glm::vec2(1150, 128), 1, "Resources/Textures/UI/Water.png");

	}

	void HandSystem::AddHandObjectsToGlobal()
	{
		GameLogicManager::Instance().AddGlobalGameObject(mpRightHand);
		GameLogicManager::Instance().AddGlobalGameObject(mpLeftHand);
		
		GameLogicManager::Instance().AddGlobalGameObject(mpRightHandUI);
		GameLogicManager::Instance().AddGlobalGameObject(mpLeftHandUI);
		
		GameLogicManager::Instance().AddGlobalGameObject(mpCombinedHandUI);
		GameLogicManager::Instance().AddGlobalGameObject(mpCombinedCooldownUI);

		for (Hollow::GameObject* pGO : mLeftHandUI)
		{
			GameLogicManager::Instance().AddGlobalGameObject(pGO);
		}
		for (Hollow::GameObject* pGO : mRightHandUI)
		{
			GameLogicManager::Instance().AddGlobalGameObject(pGO);
		}		
	}

	void HandSystem::SubscribeToEvents()
	{
		Hollow::EventManager::Instance().SubscribeEvent((int)GameEventType::CYCLE_SPELL, EVENT_CALLBACK(HandSystem::CycleSpell));
		Hollow::EventManager::Instance().SubscribeEvent((int)GameEventType::ON_SPELL_COLLECT, EVENT_CALLBACK(HandSystem::CollectSpell));
		Hollow::EventManager::Instance().SubscribeEvent((int)GameEventType::PLAYER_DEATH, EVENT_CALLBACK(HandSystem::OnPlayerDeath));
	}

	Hollow::GameObject* HandSystem::CreateUIObject(const glm::vec2& scale, const glm::vec2& position, const int layer, const std::string& texturePath, const std::string& objectFilePath)
	{
		Hollow::GameObject* UIObject = Hollow::ResourceManager::Instance().LoadGameObjectFromFile(objectFilePath);
		// Set UI position
		Hollow::UITransform* pUITr = UIObject->GetComponent<Hollow::UITransform>();
		pUITr->mScale = scale;
		pUITr->mPosition = position;
		pUITr->mLayer = layer;

		// Set UI Texture
		Hollow::UIImage* pUIImage = UIObject->GetComponent<Hollow::UIImage>();
		pUIImage->TexturePath = texturePath;
		pUIImage->mpTexture = Hollow::ResourceManager::Instance().LoadTexture(texturePath);
		pUIImage->mColor = glm::vec3(0.0f, 0.0f, 0.0f);

		return UIObject;
	}

	void HandSystem::UpdateSpellUI()
	{
		Magic* pMagic = mpPlayerObject->GetComponent<Magic>();

		// Check what spells are active and adjust the UI color for highlighting
		if (pMagic->mLeftHandSpell)
		{
			UpdateHandSpellUI("left");
		}
		else
		{
			for (unsigned int UIIndex = 0; UIIndex < mLeftHandUI.size(); ++UIIndex)
			{
				// Get current UIElement to check
				Hollow::UIImage* pUIImage = mLeftHandUI[UIIndex]->GetComponent<Hollow::UIImage>();

				// TODO: Maybe change images, maybe different color map
				pUIImage->mColor = mUncollectedColor;
			}
		}

		if (pMagic->mRightHandSpell)
		{
			UpdateHandSpellUI("right");
		}
		else
		{
			for (unsigned int UIIndex = 0; UIIndex < mRightHandUI.size(); ++UIIndex)
			{
				// Get current UIElement to check
				Hollow::UIImage* pUIImage = mRightHandUI[UIIndex]->GetComponent<Hollow::UIImage>();

				// TODO: Maybe change images, maybe different color map
				pUIImage->mColor = mUncollectedColor;
			}
		}
		
	}

	void HandSystem::UpdateHandSpellUI(const std::string& handString)
	{
		Magic* pMagic = mpPlayerObject->GetComponent<Magic>();
		int spellType = 0;
		SpellUIArray* spellUI = nullptr;

		if (handString == "left")
		{
			spellType = pMagic->mLeftHandSpell->mSpellType;
			spellUI = &mLeftHandUI;
		}
		else
		{
			spellType = pMagic->mRightHandSpell->mSpellType;
			spellUI = &mRightHandUI;
		}
		
		// Get currently selected spell index
		unsigned int selectedSpellUIIndex = (unsigned int)log2(spellType);
		for (unsigned int UIIndex = 0; UIIndex < spellUI->size(); ++UIIndex)
		{
			// Get current UIElement to check
			Hollow::UIImage* pUIImage = (*spellUI)[UIIndex]->GetComponent<Hollow::UIImage>();

			unsigned int spellMapIndex = 1 << UIIndex;
			// Normal color for collected spell
			if (pMagic->mBasicSpells[spellMapIndex]->mCollected)
			{
				// Highlight selected spell
				if (UIIndex == selectedSpellUIIndex)
				{
					pUIImage->mColor = glm::vec3(0.0f, 0.0f, 0.0f);
				}
				else
				{
					pUIImage->mColor = glm::vec3(0.0f, 0.0f, 0.0f);
				}
			}
			// Spell not collected, dim color
			else
			{
				// TODO: Maybe change images, maybe different color map
				pUIImage->mColor = mUncollectedColor;
			}
		}
	}

	void HandSystem::UpdateCombinedSpellUI()
	{
		Magic* pMagic = mpPlayerObject->GetComponent<Magic>();
		if (!mpPlayerObject->mActive)
		{
			mpPlayerObject = nullptr;
			return;
		}
		if (pMagic->mCombinedSpell != nullptr)
		{
			Hollow::UIImage* pUIImg = mpCombinedHandUI->GetComponent<Hollow::UIImage>();
			pUIImg->mpTexture = Hollow::ResourceManager::Instance().LoadTexture(pMagic->mCombinedSpell->mUITexturePath);
			pUIImg->TexturePath = pMagic->mCombinedSpell->mUITexturePath;
		}
	}

	void HandSystem::CycleSpell(Hollow::GameEvent& event)
	{
		// Convert event to cycle spell event
		CycleSpellEvent& pCycleSpellEvent = dynamic_cast<CycleSpellEvent&>(event);

		// Get spell rotation value from the player object
		Hollow::GameObject* pHandUIObj = nullptr;
		Hollow::GameObject* pHandObj = nullptr;
		Magic::SpellData* pSpellData = nullptr;
		Magic* pMagic = mpPlayerObject->GetComponent<Magic>();
		if (pCycleSpellEvent.mHandString == "left")
		{
			pHandUIObj = mpLeftHandUI;
			pSpellData = pMagic->mLeftHandSpell;
			pHandObj = mpLeftHand;
		}
		else
		{
			pHandUIObj = mpRightHandUI;
			pSpellData = pMagic->mRightHandSpell;
			pHandObj = mpRightHand;
		}

		// Update spell icon here if desired
		//

		// Update rotation of the hightlight icon
		Hollow::UITransform* pUITr = pHandUIObj->GetComponent<Hollow::UITransform>();
		pUITr->mRotation = pSpellData->mUIRotation;

		// Update light color
		Hollow::Light* pLight = pHandObj->GetComponent<Hollow::Light>();
		pLight->mColor = pSpellData->mLightColor;

		// Update hand particles
		Hollow::ParticleEmitter* pParticleEmitter = pHandObj->GetComponent<Hollow::ParticleEmitter>();
		pParticleEmitter->mDTexturePath = pSpellData->mParticleTexturePath;
		pParticleEmitter->mTexture = Hollow::ResourceManager::Instance().LoadTexture(pParticleEmitter->mDTexturePath);
	}

	void HandSystem::CollectSpell(Hollow::GameEvent& event)
	{
		SetHighlightUIActive(true);
		mpLeftHandUI->GetComponent<Hollow::UITransform>()->mLayer = 2;
		mpRightHandUI->GetComponent<Hollow::UITransform>()->mLayer = 2;
	}

	void HandSystem::OnPlayerDeath(Hollow::GameEvent& event)
	{
		// Reset hand UI images
		mpCombinedHandUI->GetComponent<Hollow::UIImage>()->mpTexture = Hollow::ResourceManager::Instance().LoadTexture("Resources/Textures/UI/Combined.png");
		SetHighlightUIActive(false);
	}

	void HandSystem::SetHighlightUIActive(bool active)
	{
		mpCombinedCooldownUI->mActive = active;
		mpLeftHandUI->mActive = active;
		mpRightHandUI->mActive = active;
	}

}