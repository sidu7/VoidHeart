#include "GameLogicManager.h"

#include "Hollow/Managers/SceneManager.h"
#include "Hollow/Managers/ScriptingManager.h"
#include "Hollow/Managers/SystemManager.h"
#include "Hollow/Managers/ResourceManager.h"
#include "Hollow/Managers/EventManager.h"
#include "Hollow/Managers/GameObjectManager.h"
#include "Hollow/Managers/EventManager.h"

#include "Hollow/Components/Script.h"
#include "Hollow/Components/Transform.h"

#include "Components/Attack.h"
#include "Components/Pickup.h"
#include "Components/Health.h"
#include "Components/CharacterStats.h"

#include "DungeonGeneration/DungeonRoom.h"
#include "DungeonGeneration/DungeonManager.h"
#include "Hollow/Managers/EventManager.h"
#include "Hollow/Managers/GameObjectManager.h"
#include "GameMetaData/GameEventType.h"
#include "GameMetaData/GameObjectType.h"
#include "Events/PickupTimedEvent.h"


#define MAX_REGULAR_ROOMS 8
#define MAX_BOSS_ROOMS 2

namespace BulletHell
{
    GameLogicManager::GameLogicManager() 
	{
		// Preload all rooms into map
		InitializeRoomsMap();
		mPickupPrefabNames.push_back("Pickup_Damage");
		mPickupPrefabNames.push_back("Pickup_HP");
		mPickupPrefabNames.push_back("Pickup_Invincible");
		mPickupPrefabNames.push_back("Pickup_RateOfFire");
		mPickupPrefabNames.push_back("Pickup_Speed");
		std::cout << "Initialized: GameLogicManager" << std::endl; 
	}

    GameLogicManager& GameLogicManager::Instance()
    {
        static GameLogicManager instance;
        return instance;
    }

    void GameLogicManager::Init()
    {
		SubscribeToEvents();
        Hollow::SceneManager::Instance().LoadLevel("Level3");
        Hollow::ScriptingManager::Instance().RunScript("GameConfig");


        BulletHell::DungeonManager::Instance().ConfigureDungeon();
        BulletHell::DungeonManager::Instance().Init();


        Hollow::ScriptingManager::Instance().RunScript("SetupLevel");

        BulletHell::DungeonManager::Instance().mpPlayerGo = Hollow::ScriptingManager::Instance().lua["player"];
        Hollow::SystemManager::Instance().OnSceneInit();


		mRandomCount = 3; // first drop after 3 enemies... then randomize
		mCountDeadEnemies = 0;

		Hollow::EventManager::Instance().SubscribeEvent((int)GameEventType::DEATH, EVENT_CALLBACK(GameLogicManager::DropRandomPickup));
    }

    void GameLogicManager::MoveToNextFloor()
    {

        auto& lua = Hollow::ScriptingManager::Instance().lua;
        int currentFloor = lua["currentFloor"].get<int>();
        DungeonManager::Instance().GetFloor(currentFloor).ResetFloor();
        currentFloor++;
        lua["currentFloor"] = currentFloor;
        Hollow::SceneManager::Instance().LoadLevel("Level3");
        DungeonManager::Instance().Construct(currentFloor);

        Hollow::ScriptingManager::Instance().RunScript("SetupLevel");
        
        BulletHell::DungeonManager::Instance().mpPlayerGo = Hollow::ScriptingManager::Instance().lua["player"];
        Hollow::SystemManager::Instance().OnSceneInit();
    }

    void GameLogicManager::SubscribeToEvents()
    {
		Hollow::EventManager::Instance().SubscribeEvent((int)GameEventType::ROOM_LOCKDOWN_DELAYED, EVENT_CALLBACK(GameLogicManager::OnRoomLockDownDelayed));
		Hollow::EventManager::Instance().SubscribeEvent((int)GameEventType::ON_PICKUP_COLLECT, EVENT_CALLBACK(GameLogicManager::OnPickupCollected));
		Hollow::EventManager::Instance().SubscribeEvent((int)GameEventType::ON_PICKUP_EFFECT_END, EVENT_CALLBACK(GameLogicManager::OnPickupEffectEnd));
		Hollow::EventManager::Instance().SubscribeEvent((int)GameEventType::ON_BULLET_HIT_SHIELD, EVENT_CALLBACK(GameLogicManager::OnBulletHitShield));
    }

	void GameLogicManager::OnPickupCollected(Hollow::GameEvent& event)
	{
		// Find out the pickup gameobject 
		Hollow::GameObject* pPickupObject = event.mpObject1->mType == (int)GameObjectType::PICKUP ? event.mpObject1 : event.mpObject2;
		Hollow::GameObject* pPlayer = event.mpObject1->mType == (int)GameObjectType::PLAYER ? event.mpObject1 : event.mpObject2;

		Pickup* pPickup = pPickupObject->GetComponent<Pickup>();
		AddBuffs(pPlayer, pPickup);

		if (pPickup->mEffectTime > 0.0f)
		{
			float effectTime = pPickup->mEffectTime;

			// change pickup so that its effects are reversed after the given time
			pPickup->mEffectTime = 0.0f;
			pPickup->mBuffValue = -pPickup->mBuffValue;
			
			// create a pickup timed event
			PickupTimedEvent* pEvent = new PickupTimedEvent(pPickup);
			pEvent->mpObject1 = pPlayer;

			Hollow::EventManager::Instance().AddDelayedEvent(pEvent, effectTime);

		}
		Hollow::GameObjectManager::Instance().DeleteGameObject(pPickupObject);
	}

	void GameLogicManager::OnPickupEffectEnd(Hollow::GameEvent& event)
	{
		PickupTimedEvent* pte = dynamic_cast<PickupTimedEvent*>(&event);

		// Add the buffs to the object
		AddBuffs(pte->mpObject1, &pte->mPickup);
	}

    Hollow::GameObject* GameLogicManager::GenerateObjectAtPosition(std::string prefabName, glm::ivec2 roomCoords, glm::vec2 posOffset)
    {
		Hollow::GameObject* pGo = Hollow::ResourceManager::Instance().LoadPrefabAtPosition(prefabName,
			glm::vec3(roomCoords.y * DungeonRoom::mRoomSize + posOffset.x,
				1.5f,
				roomCoords.x * DungeonRoom::mRoomSize + posOffset.y));

        // TODO adjust enemy level according to dungeon floor

        // Disable any scripts
        Hollow::Script* pS = pGo->GetComponent<Hollow::Script>();
		if (pS != nullptr)
		{
			pS->mIsActive = false;
		}
        Attack* pA = pGo->GetComponent<Attack>();
		if (pA != nullptr)
		{
			pA->mIsActive = false;
		}

        return pGo;
    }

    void GameLogicManager::OnRoomLockDownDelayed(Hollow::GameEvent& event)
    {
		auto room = DungeonManager::Instance().GetCurrentRoom();
		for (auto enemy : room.mEnemies)
		{
			// Enable any scripts
			Hollow::Script* pS = enemy->GetComponent<Hollow::Script>();
			if (pS != nullptr)
			{
				pS->mIsActive = true;
			}
			Attack* pA = enemy->GetComponent<Attack>();
			if (pA != nullptr)
			{
				pA->mIsActive = true;
			}
		}
    }

    void GameLogicManager::InitializeRoomsMap()
	{
		// Loads all room files into map
		// for all room files
		LoadRoomJsons("Room", MAX_REGULAR_ROOMS);

		// Do the same for boss rooms
		LoadRoomJsons("Boss/Room", MAX_BOSS_ROOMS);
	}

	void GameLogicManager::AddBuffs(Hollow::GameObject* pPlayer, Pickup* pPickup)
	{
		CharacterStats* pStats = pPlayer->GetComponent<CharacterStats>();

		switch (pPickup->mPickupType)
		{
		case PickupType::HP:
		{
			Health* pHp = pPlayer->GetComponent<Health>();
			pHp->mHitPoints += pPickup->mBuffValue;
			break;
		}
		case PickupType::DASH:
		{
			pStats->mDashSpeed += pPickup->mBuffValue;
			break;
		}
		case PickupType::DAMAGE_FACTOR:
		{
			pStats->mDamageFactor += pPickup->mBuffValue;
			break;
		}
		case PickupType::SPEED_FACTOR:
		{
			pStats->mMovementSpeedFactor += pPickup->mBuffValue;
			break;
		}
		case PickupType::SPEED:
		{
			pStats->mMovementSpeed += pPickup->mBuffValue;
			break;
		}
		case PickupType::RATE_OF_FIRE:
		{
			pStats->mFireRate += pPickup->mBuffValue;
			break;
		}
		case PickupType::INVINCIBILITY:
		{
			Health* pHp = pPlayer->GetComponent<Health>();
			pHp->mInvincibleTime += pPickup->mBuffValue;
			break;
		}
		}
	}

	void GameLogicManager::LoadRoomJsons(std::string roomPrefix, int count)
	{
		std::string path = "Resources/Json data/Rooms/";
		for (int i = 1; i <= count; ++i)
		{
			std::string name = roomPrefix + std::to_string(i);
			std::ifstream file(path + name + ".json");
			std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			file.close();
			mCachedRoomsMap[name] = contents;
		}
	}

	void GameLogicManager::OnBulletHitShield(Hollow::GameEvent& event)
	{
		// Destroy the enemy bullet and spawn a new player bullet that tracks the nearest enemy
		Hollow::GameObject* pBullet = event.mpObject1->mType == (int)GameObjectType::BULLET ? event.mpObject1 : event.mpObject2;
		Hollow::ScriptingManager::Instance().RunScript("CreateWater", pBullet);
		Hollow::GameObjectManager::Instance().DeleteGameObject(pBullet);
	}

	void GameLogicManager::PopulateRoom(DungeonRoom& room)
	{
		int roomNum = 0; 
		std::string roomName;
		if(room.mRoomType == DungeonRoomType::BOSS)
		{
			roomNum = Random::RangeSeeded(1, MAX_BOSS_ROOMS);
			roomName = "Boss/Room" + std::to_string(roomNum);
		}
		else
		{
			roomNum = Random::RangeSeeded(1, MAX_REGULAR_ROOMS);
			roomName = "Room" + std::to_string(roomNum);
		}
		
		rapidjson::Document root;
		root.Parse(mCachedRoomsMap[roomName].c_str());
		if (!root.IsObject()) { HW_CORE_ERROR("Error reading Room file {0}", roomName); }
		rapidjson::Value::Object data = root.GetObject();

		if(data.HasMember("Enemies"))
		{
			CreateObjectsInRoom(room, data, "Enemies", "EnemyType", true);
		}

		if(data.HasMember("Obstacles"))
		{
			CreateObjectsInRoom(room, data, "Obstacles", "ObstacleType", false);
		}
		
	}

	void GameLogicManager::CreateObjectsInRoom(DungeonRoom& room, const rapidjson::Value::Object& data, const std::string& objects, const std::string& type, bool addToEnemylist)
	{
		glm::ivec2 coords = room.GetCoords();
		// Parse all objects in file
		std::string prefabName = "";
		glm::vec2 position(0.0f, 0.0f);
		if (data[objects.c_str()].IsArray())
		{
			auto objectArray = data[objects.c_str()].GetArray();
			for (int objectIndex = 0; objectIndex < objectArray.Size(); ++objectIndex)
			{
				auto object = objectArray[objectIndex].GetObject();
				// Get object type, this is the json prefab file that will be loaded
				if (object[type.c_str()].IsString())
				{
					prefabName = object[type.c_str()].GetString();
				}
				// Get object position
				if (object["Position"].IsArray())
				{
					auto pos = object["Position"].GetArray();
					position = glm::vec2(pos[0].GetFloat(), pos[1].GetFloat());
				}

				// Load enemy prefab at position relative to room
				Hollow::GameObject* pGo = GenerateObjectAtPosition(prefabName, coords, position);
				if (addToEnemylist)
				{
					room.mEnemies.push_back(pGo);
				}
			}
		}
	}

	void GameLogicManager::DropRandomPickup(Hollow::GameEvent& event)
	{
		if (event.mpObject1->mType == (int)GameObjectType::ENEMY)
		{
			mCountDeadEnemies++;

			if (mCountDeadEnemies > mRandomCount)
			{
				mCountDeadEnemies = 0;
				mRandomCount = Random::RangeSeeded(5, 9);

				int randomIndex = Random::RangeSeeded(0, mPickupPrefabNames.size()-1);
				
				Hollow::Transform* pTr = event.mpObject1->GetComponent<Hollow::Transform>();
				Hollow::ResourceManager::Instance().LoadPrefabAtPosition(mPickupPrefabNames[randomIndex], pTr->mPosition);
			}
		}
	}

    void GameLogicManager::CreatePickUpInRoom(DungeonRoom& room)
    {
        glm::ivec2 coords = room.GetCoords();
		int randomIndex = Random::RangeSeeded(0, mPickupPrefabNames.size()-1);

        Hollow::GameObject* pGo = Hollow::ResourceManager::Instance().LoadPrefabAtPosition(
				mPickupPrefabNames[randomIndex],
             glm::vec3(coords.y * DungeonRoom::mRoomSize + DungeonRoom::mRoomSize / 2,
                1.5,
                coords.x * DungeonRoom::mRoomSize + DungeonRoom::mRoomSize / 2));

    	// make the effect time zero ie make it permanent
    	Pickup* pP = pGo->GetComponent<Pickup>();
		pP->mEffectTime = 0.0f;
    	// make the effect stronger
		pP->mBuffValue = Random::RangeSeeded(1.2f, 2.0f) * pP->mBuffValue;
    }
}
