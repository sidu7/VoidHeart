#include <Hollow.h>
#include "HealthSystem.h"

#include "Hollow/Managers/EventManager.h"
#include "Hollow/Managers/GameObjectManager.h"
#include "Hollow/Managers/InputManager.h"
#include "Hollow/Managers/FrameRateController.h"
#include "Hollow/Managers/AudioManager.h"
#include "Hollow/Managers/ResourceManager.h"
#include "Hollow/Managers/LocalizationManager.h"

#include "Hollow/Components/UIText.h"

#include "GameMetaData/GameEventType.h"
#include "GameMetaData/GameObjectType.h"

#include "Components/Health.h"

namespace BulletHell
{
	HealthSystem HealthSystem::instance;

	void HealthSystem::Init()
	{
		// Set event callback functions
		Hollow::EventManager::Instance().SubscribeEvent((int)GameEventType::ON_BULLET_HIT_PLAYER, EVENT_CALLBACK(HealthSystem::OnBulletHitPlayer));
		Hollow::EventManager::Instance().SubscribeEvent((int)GameEventType::ON_BULLET_HIT_WALL, EVENT_CALLBACK(HealthSystem::OnBulletHitWall));
		Hollow::EventManager::Instance().SubscribeEvent((int)GameEventType::ON_PLAYER_BULLET_HIT_ENEMY, EVENT_CALLBACK(HealthSystem::OnPlayerBulletHitEnemy));
	}

	void HealthSystem::Update()
	{
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			Health* pHealth = mGameObjects[i]->GetComponent<Health>();

			// Update player HP bar
			Hollow::UIText* pHPText = mGameObjects[i]->GetComponent<Hollow::UIText>();
			//std::string ss = Hollow::LocalizationManager::Instance().mCurrentLanguageMap[pHPText->mTag];
			//pHPText->mText = ss + std::to_string(pHealth->mHitPoints);
			
			// Update invincibility time
			if (pHealth->mInvincible)
			{
				pHealth->mCurrentInvincibleTime += Hollow::FrameRateController::Instance().GetFrameTime();
				
				// Check if should still be invincible
				if (pHealth->mCurrentInvincibleTime > pHealth->mInvincibleTime)
				{
					pHealth->mInvincible = false;
				}
			}

			// Check for invincibility
			if (Hollow::InputManager::Instance().IsControllerTriggerTriggered(SDL_CONTROLLER_AXIS_TRIGGERLEFT)
				&& !pHealth->mInvincible && pHealth->mpOwner->mType == (int)GameObjectType::PLAYER)
			{
				pHealth->mInvincible = true;
				pHealth->mCurrentInvincibleTime = 0.0f;
			}

			if (pHealth->mHitPoints < 0)
			{
				pHealth->mIsAlive = false;
				std::string gameEndText = "";

				// Check type of object destroyed
				if (mGameObjects[i]->mType == (int)GameObjectType::PLAYER)
				{
					//YOU LOSE
					Hollow::AudioManager::Instance().PlayEffect("Resources/Audio/SFX/lose.wav");
					//gameEndText = "You Lose!";
					gameEndText = Hollow::LocalizationManager::Instance().mCurrentLanguageMap["LOSE"];
				}
				if (mGameObjects[i]->mType == (int)GameObjectType::ENEMY)
				{
					//YOU WIN
					Hollow::AudioManager::Instance().PlayEffect("Resources/Audio/SFX/win.wav");
					//gameEndText = "You Win!";
					gameEndText = Hollow::LocalizationManager::Instance().mCurrentLanguageMap["WIN"];
				}
				// Send event to destroy object
				Hollow::GameObjectManager::Instance().DeleteGameObject(mGameObjects[i]);

				// Create new UI object
				// TODO: Fix how this is being done
				/*Hollow::GameObject* pUI = Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Json Data/UIElement.json");
				Hollow::UIText* pUIText = pUI->GetComponent<Hollow::UIText>();
				pUIText->mOffsetPosition = glm::vec2(400.0f, 300.0f);
				pUIText->mTextScale = glm::vec2(4.0f, 4.0f);
				pUIText->mText = gameEndText;
				pUIText->mChangingText = true;*/
			}
		}
	}

	void HealthSystem::AddGameObject(Hollow::GameObject* pGameObject)
	{
		CheckAllComponents<Health>(pGameObject);
	}

	void HealthSystem::HandleBroadcastEvent(Hollow::GameEvent& event)
	{
	}

	void HealthSystem::OnBulletHitPlayer(Hollow::GameEvent& event)
	{
		if (event.mpObject1->mType == (int)GameObjectType::PLAYER)
		{
			// Call handle function with player, bullet
			HandleBulletDamage(event.mpObject1, event.mpObject2);
		}
		else
		{
			// Call handle function with input reversed
			HandleBulletDamage(event.mpObject2, event.mpObject1);
		}
				
		Hollow::AudioManager::Instance().PlayEffect("Resources/Audio/SFX/PlayerHit.wav");
	}

	void HealthSystem::OnBulletHitWall(Hollow::GameEvent& event)
	{
		if (event.mpObject1->mType == (int)GameObjectType::WALL)
		{
			Hollow::GameObjectManager::Instance().DeleteGameObject(event.mpObject2);
		}
		else
		{
			Hollow::GameObjectManager::Instance().DeleteGameObject(event.mpObject1);
		}
	}

	void HealthSystem::OnPlayerBulletHitEnemy(Hollow::GameEvent& event)
	{		
		if (event.mpObject1->mType == (int)GameObjectType::ENEMY)
		{
			HandleBulletDamage(event.mpObject1, event.mpObject2);
		}
		else
		{
			HandleBulletDamage(event.mpObject2, event.mpObject1);
		}
		Hollow::AudioManager::Instance().PlayEffect("Resources/Audio/SFX/BossHit.wav");
	}

	void HealthSystem::HandleBulletDamage(Hollow::GameObject* pObjectHit, Hollow::GameObject* pBullet)
	{
		// Destroy player bullet
		Hollow::GameObjectManager::Instance().DeleteGameObject(pBullet);

		// Decrease player health, object hit must have a health component
		Health* pHealth = pObjectHit->GetComponent<Health>();
		if (!pHealth->mInvincible)
		{
			--pHealth->mHitPoints;
		}		
	}

}