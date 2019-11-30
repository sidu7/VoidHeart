#include <Hollow.h>
#include "HealthSystem.h"

#include "Hollow/Managers/EventManager.h"
#include "Hollow/Managers/GameObjectManager.h"

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
			if (pHealth->mHitPoints < 0)
			{
				pHealth->mIsAlive = false;
			}
			if (!pHealth->mIsAlive)
			{
				// Send event to destroy object
				Hollow::GameObjectManager::Instance().DeleteGameObject(mGameObjects[i]);
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
	}

	void HealthSystem::OnBulletHitWall(Hollow::GameEvent& event)
	{
		if (event.mpObject1->mType == (int)GameObjectType::BULLET)
		{
			Hollow::GameObjectManager::Instance().DeleteGameObject(event.mpObject1);
		}
		else if (event.mpObject2->mType == (int)GameObjectType::BULLET)
		{
			Hollow::GameObjectManager::Instance().DeleteGameObject(event.mpObject2);
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
	}

	void HealthSystem::HandleBulletDamage(Hollow::GameObject* pObjectHit, Hollow::GameObject* pBullet)
	{
		// Destroy player bullet
		Hollow::GameObjectManager::Instance().DeleteGameObject(pBullet);

		// Decrease player health, object hit must have a health component
		Health* pHealth = pObjectHit->GetComponent<Health>();
		--pHealth->mHitPoints;
	}

}