#include <Hollow.h>
#include "HealthSystem.h"

#include "Hollow/Managers/EventManager.h"
#include "Hollow/Managers/GameObjectManager.h"
#include "Hollow/Managers/InputManager.h"
#include "Hollow/Managers/FrameRateController.h"
#include "Hollow/Managers/AudioManager.h"
#include "Hollow/Managers/ResourceManager.h"
#include "Hollow/Managers/LocalizationManager.h"
#include "Hollow/Managers/PhysicsManager.h"

#include "Hollow/Components/UIText.h"
#include "Hollow/Components/Transform.h"
#include "Hollow/Components/UIImage.h"
#include "Hollow/Components/UITransform.h"

#include "GameMetaData/GameEventType.h"
#include "GameMetaData/GameObjectType.h"

#include "Components/Health.h"
#include "Events/DeathEvent.h"

namespace BulletHell
{
	HealthSystem HealthSystem::instance;

	void HealthSystem::Init()
	{
		// Set event callback functions
		Hollow::EventManager::Instance().SubscribeEvent((int)GameEventType::ON_BULLET_HIT_PLAYER, EVENT_CALLBACK(HealthSystem::OnBulletHitPlayer));
		Hollow::EventManager::Instance().SubscribeEvent((int)GameEventType::ON_BULLET_HIT_WALL, EVENT_CALLBACK(HealthSystem::OnBulletHitWall));
		Hollow::EventManager::Instance().SubscribeEvent((int)GameEventType::ON_BULLET_HIT_DOOR, EVENT_CALLBACK(HealthSystem::OnBulletHitDoor));
		Hollow::EventManager::Instance().SubscribeEvent((int)GameEventType::ON_PLAYER_BULLET_HIT_ENEMY, EVENT_CALLBACK(HealthSystem::OnPlayerBulletHitEnemy));
		Hollow::EventManager::Instance().SubscribeEvent((int)GameEventType::ON_ENEMY_AOE_DAMAGE_HIT_PLAYER, EVENT_CALLBACK(HealthSystem::OnAOEDamageHitPlayer));
		//Hollow::EventManager::Instance().SubscribeEvent((int)GameEventType::ON_PLAYER_BULLET_HIT_ENEMY, EVENT_C)
	}

	void HealthSystem::Update()
	{
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			Health* pHealth = mGameObjects[i]->GetComponent<Health>();

			// Update player HP bar
			//Hollow::UIText* pHPText = mGameObjects[i]->GetComponent<Hollow::UIText>();
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

				DeathEvent death(mGameObjects[i]->mType);
				death.mpObject1 = mGameObjects[i];
				Hollow::EventManager::Instance().BroadcastToSubscribers(death);
				
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
			}
			// Create new UI object
			//if (mGameObjects[i]->mTag == "Player")
			if (mGameObjects[i]->mType == (int)GameObjectType::PLAYER)
			{
				// Populate UIIcons
				if (mPlayerHPUIIcons.empty())
				{
					int numUIIcons = pHealth->mHitPoints / 2;
					for (int i = 0; i < numUIIcons; ++i)
					{
						CreateHPUIIcon(i);
					}
				}
				else
				{
					int playerHealth = pHealth->mHitPoints;
					if (playerHealth > mPlayerHPUIIcons.size()*2)
					{
						CreateHPUIIcon(mPlayerHPUIIcons.size());
					}
					for (int UIIndex = 0; UIIndex < mPlayerHPUIIcons.size(); ++UIIndex)
					{
						Hollow::GameObject* pUIIcon = mPlayerHPUIIcons[UIIndex];
						Hollow::UIImage* pUIImg = pUIIcon->GetComponent<Hollow::UIImage>();
						// Play should have full hp
						if (playerHealth >= (UIIndex + 1)*2)
						{
							pUIImg->TexturePath = "Resources/Textures/HPIcon.png";
						}
						else if (playerHealth <= (UIIndex) * 2)
						{
							pUIImg->TexturePath = "Resources/Textures/EmptyHPIcon.png";
						}
						else
						{
							pUIImg->TexturePath = "Resources/Textures/HalfHPIcon.png";
						 }
						pUIImg->mpTexture = Hollow::ResourceManager::Instance().LoadTexture(pUIImg->TexturePath);
					}
					//Hollow::UIImage* pUIImg = mpPlayerHPUI->GetComponent<Hollow::UIImage>();
					//pUIImg->TexturePath = "Resources/Textures/HalfHPIcon.png";
					//pUIImg->mpTexture = Hollow::ResourceManager::Instance().LoadTexture(pUIImg->TexturePath);
				}
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
		Hollow::GameObject* pPlayer = nullptr;
		if (event.mpObject1->mType == (int)GameObjectType::PLAYER)
		{
			// Call handle function with player, bullet
			HandleBulletDamage(event.mpObject1, event.mpObject2);
			pPlayer = event.mpObject1;
		}
		else
		{
			// Call handle function with input reversed
			HandleBulletDamage(event.mpObject2, event.mpObject1);
			pPlayer = event.mpObject2;
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

    void HealthSystem::OnBulletHitDoor(Hollow::GameEvent& event)
    {
        if (event.mpObject1->mType == (int)GameObjectType::DOOR)
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

	void HealthSystem::OnAOEDamageHitPlayer(Hollow::GameEvent& event)
	{
		Hollow::GameObject* gameobject;
		Hollow::GameObject* aoe;
		if (event.mpObject1->mType == (int)GameObjectType::PLAYER)
		{
			gameobject = event.mpObject1;
			aoe = event.mpObject2;
		}
		else
		{
			gameobject = event.mpObject2;
			aoe = event.mpObject1;
		}
		Health* pHealth = gameobject->GetComponent<Health>();
		if (!pHealth->mInvincible)
		{
			--pHealth->mHitPoints;
		}
		glm::vec3 impulse = glm::vec3(0.0f);
		glm::vec3 aoe_pos = aoe->GetComponent<Hollow::Transform>()->mPosition;
		glm::vec3 player_pos = gameobject->GetComponent<Hollow::Transform>()->mPosition;
		glm::vec3 direction = glm::normalize(player_pos - aoe_pos);
		impulse = direction * 10000.0f;
		Hollow::PhysicsManager::Instance().ApplyLinearImpulse(gameobject, impulse);
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

	void HealthSystem::CreateHPUIIcon(int index)
	{
		Hollow::GameObject* pUIObj = Hollow::ResourceManager::Instance().LoadGameObjectFromFile("Resources/Prefabs/UIIcon.json");
		//Hollow::UIText* pUIText = pUI->GetComponent<Hollow::UIText>();
		Hollow::UIImage* pUIImg = pUIObj->GetComponent<Hollow::UIImage>();
		pUIImg->TexturePath = "Resources/Textures/HPIcon.png";
		pUIImg->mpTexture = Hollow::ResourceManager::Instance().LoadTexture(pUIImg->TexturePath);

		Hollow::UITransform* pUITr = pUIObj->GetComponent<Hollow::UITransform>();
		pUITr->mScale = glm::vec2(64, 64);
		pUITr->mPosition = glm::vec2(64 * (index + 1), 660);
		mPlayerHPUIIcons.push_back(pUIObj);
	}

}
